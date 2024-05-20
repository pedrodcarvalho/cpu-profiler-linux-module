#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>

#include "cpu_profiler.h"

// Module metadata
MODULE_LICENSE("GPL");
MODULE_AUTHOR("CPU Profiler Linux Module");
MODULE_DESCRIPTION("Simple CPU Profiler Linux Kernel Module");
MODULE_VERSION("1.0");

// Debug flag
#define DEBUG 1

// Name of the proc filesystem entry
#define procfs_name "log"

// Maximum size of the buffer (4 bytes)
#define INCOMMING_MAX_SIZE 4

// Timer flags
#define TIMER_SET 0
#define TIMER_NOT_SET 1

// Spinlock
DEFINE_SPINLOCK(module_read_lock);

// Timer
static struct timer_list timer_5sec;
unsigned int timer_set;

// Head of the linked list
LIST_HEAD(cpu_profiler_linked_list);

// Structure to store the PID and the CPU time
struct user_time_pid {
    unsigned long pid;
    unsigned long user_time;
    struct list_head cpu_profiler_list;
};

/* Methods */

// Workqueue used to process the list of processes
static struct workqueue_struct *process_workqueue;

struct cpu_profiler_work {
    struct work_struct my_work;
};

struct cpu_profiler_work *work;

static void process_workqueue_function(struct work_struct *work)
{
    int ret;
    struct list_head *pos, *q;
    struct user_time_pid *tmp;

    printk("Function called by the workqueue\n");

    // Lock the list to avoid inconsistency
    spin_lock(&module_read_lock);

    // Iterate over the list containing the processes and check if they still exist
    list_for_each_safe(pos, q, &cpu_profiler_linked_list)
    {
        tmp = list_entry(pos, struct user_time_pid, cpu_profiler_list);
        // Using the get_cpu_use function to check if the process still exists
        ret = get_cpu_use(tmp->pid, &tmp->user_time);

        if (ret) {
            printk("Process %ld does not exist anymore. Removing it...\n", tmp->pid);
            list_del(pos);
            kfree(tmp);
        }
    }

    // Unlock the list
    spin_unlock(&module_read_lock);

    return;
}

void timer_5sec_callback(struct timer_list *data)
{
    // We only queue work if we have a process in the list
    if (0 == list_empty(&cpu_profiler_linked_list)) {
        queue_work(process_workqueue, (struct work_struct *)work);

        // Set the timer again
        mod_timer(&timer_5sec, jiffies + msecs_to_jiffies(5000));
    }
    else {
        timer_set = TIMER_NOT_SET;
    }
}

// Function to display proc file content
static int log_proc_show(struct seq_file *m, void *v)
{
    struct user_time_pid *tmp = NULL;

    // Lock the list to avoid inconsistency
    spin_lock(&module_read_lock);

    list_for_each_entry(tmp, &cpu_profiler_linked_list, cpu_profiler_list)
    {
        struct task_struct *task;

        // Use the find_task_by_pid macro to get the task_struct of the process
        task = find_task_by_pid(tmp->pid);

        int minutes = tmp->user_time / 60;
        int seconds = tmp->user_time % 60;

        // Check if the process still exists. If so, print the PID, process name, and CPU time in the format MM:SS
        if (task != NULL) {
            // Using seq_printf because we are in the context of the seq_file
            seq_printf(m, "PID: %*ld | Process Name: %*s | CPU Time: %*i:%02i\n",
                       15, tmp->pid,
                       15, task->comm,
                       15, minutes, seconds);
        }
    }

    spin_unlock(&module_read_lock);

    return 0;
}

static int log_proc_open(struct inode *inode, struct file *file)
{
    printk("Calling log_proc_open.\n");
    return single_open(file, log_proc_show, NULL);
}

ssize_t procfile_write(struct file *file, const char *buffer, size_t count, loff_t *offset)
{
    int ret;
    static size_t buffer_size = 0;
    struct user_time_pid *new_process;
    new_process = kmalloc(sizeof(struct user_time_pid), GFP_KERNEL);
    // Dummy initialization
    new_process->pid = 0;

    // Check if the buffer size is bigger than the maximum size
    buffer_size = count;

    if (buffer_size > INCOMMING_MAX_SIZE) {
        buffer_size = INCOMMING_MAX_SIZE;
    }

    // Copy the buffer to the new_process->pid
    if (copy_from_user((void *)&(new_process->pid), buffer, buffer_size)) {
        return -EFAULT;
    }

    ret = get_cpu_use(new_process->pid, &new_process->user_time);

    if (ret) {
        printk("Process %ld does not exist.\n", new_process->pid);
        kfree((void *)new_process);
        return -1;
    }
    else {
        // Lock the list to avoid inconsistency
        spin_lock(&module_read_lock);
        list_add(&new_process->cpu_profiler_list, &cpu_profiler_linked_list);
        spin_unlock(&module_read_lock);

        printk("Received: %ld \n", new_process->pid);

        // Set the timer if it's not set
        if (timer_set == TIMER_NOT_SET) {
            ret = mod_timer(&timer_5sec, jiffies + msecs_to_jiffies(5000));
            timer_set = TIMER_SET;
        }
    }

    return buffer_size;
}

// Structure that defines the operations that can be performed on the proc file
static const struct proc_ops log_proc_ops = {
    .proc_open = log_proc_open,
    .proc_read = seq_read,
    .proc_write = procfile_write,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

// This is the initialization method of the module
int __init cpu_profiler_init(void)
{
    struct proc_dir_entry *parent_proc;
    struct proc_dir_entry *proc_write_entry;

#ifdef DEBUG
    printk(KERN_ALERT "CPU Profiler MODULE LOADING\n");
#endif

    // Creating the proc filesystem entry
    parent_proc = proc_mkdir("cpu_profiler", NULL);
    proc_write_entry = proc_create("log", 0666, parent_proc, &log_proc_ops);

    // Initialize the timer
    timer_setup(&timer_5sec, timer_5sec_callback, 0);

    // The timer will only be set when we have a process in the list
    timer_set = TIMER_NOT_SET;

    // Initialize the workqueue
    process_workqueue = create_workqueue("process_workqueue");

    if (process_workqueue) {
        // Queue work to the workqueue
        work = (struct cpu_profiler_work *)kmalloc(sizeof(struct cpu_profiler_work), GFP_KERNEL);

        if (work) {
            INIT_WORK((struct work_struct *)work, process_workqueue_function);
        }
    }

    printk(KERN_ALERT "CPU Profiler MODULE LOADED\n");
    return 0;
}

// This is the exit method of the module
void __exit cpu_profiler_exit(void)
{
    int ret;
    struct list_head *pos, *q;
    struct user_time_pid *tmp;

#ifdef DEBUG
    printk(KERN_ALERT "CPU Profiler MODULE UNLOADING\n");
#endif

    // Deleting timer
    ret = del_timer(&timer_5sec);

    if (ret) {
        printk("Deleting an active timer\n");
    }

    // Deleting workqueue
    kfree((void *)work);
    flush_workqueue(process_workqueue);
    destroy_workqueue(process_workqueue);

    /*
     * Freeing the user_time_pid items requires caution, as items will be removed
     * from the list using list_del(). To ensure safe iteration during deletions
     * or movements of items between lists, the macro list_for_each_safe() must
     * be utilized instead of list_for_each(). It is imperative to adhere to this
     * protocol when manipulating the list to prevent unexpected behavior.
     */

    list_for_each_safe(pos, q, &cpu_profiler_linked_list)
    {
        tmp = list_entry(pos, struct user_time_pid, cpu_profiler_list);
        printk("freeing item pid= %ld \n", tmp->pid);
        list_del(pos);
        kfree(tmp);
    }

    // Removing proc filesystem entry
    remove_proc_entry("cpu_profiler/log", NULL);
    remove_proc_entry("cpu_profiler", NULL);

    printk(KERN_ALERT "CPU Profiler MODULE UNLOADED\n");
}

// Register the initialization and exit methods
module_init(cpu_profiler_init);
module_exit(cpu_profiler_exit);
