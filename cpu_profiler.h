#ifndef __CPU_PROFILER_INCLUDE__
#define __CPU_PROFILER_INCLUDE__

#include <linux/pid.h>
#include <linux/kthread.h>
#include <linux/sched.h>

#define find_task_by_pid(nr) pid_task(find_vpid(nr), PIDTYPE_PID)

// Checks if the PID is valid, if so returns the CPU time of the process in milliseconds, otherwise returns -1
int get_cpu_use(int pid, unsigned long *cpu_use)
{
    struct task_struct *task;

    rcu_read_lock();

    task = find_task_by_pid(pid);

    if (task != NULL) {
        *cpu_use = jiffies_to_msecs(clock_t_to_jiffies(task->utime));
        rcu_read_unlock();
        return 0;
    }
    else {
        rcu_read_unlock();
        return -1;
    }
}

#endif
