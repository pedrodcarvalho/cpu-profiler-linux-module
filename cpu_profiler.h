// Include guard to avoid multiple inclusion
#ifndef __CPU_PROFILER_INCLUDE__
#define __CPU_PROFILER_INCLUDE__

#include <linux/pid.h>
#include <linux/sched.h>

// Compiler complains about missing prototypes
int get_cpu_use(int pid, unsigned long *cpu_use);

// Macro to simplify the task lookup. It returns the task_struct of the process with the given PID
#define find_task_by_pid(nr) pid_task(find_vpid(nr), PIDTYPE_PID)

// Checks if the PID is valid, if so returns the CPU time of the process in milliseconds, otherwise returns -1
int get_cpu_use(int pid, unsigned long *cpu_use)
{
    struct task_struct *task;

    // Acquire the RCU read lock to safely access the task_struct
    rcu_read_lock();

    task = find_task_by_pid(pid);

    if (task != NULL) {
        // Calculate the CPU (from user and system) time in seconds
        *cpu_use = (task->utime + task->stime) / 1000000000;
        rcu_read_unlock();
        return 0;
    }
    else {
        rcu_read_unlock();
        return -1;
    }
}

#endif
