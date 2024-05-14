#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#ifndef DT_DIR
#define DT_DIR 4
#endif

struct top_process_info {
    pid_t pid;
    unsigned long cpu_time;
};

// Function to parse CPU time from /proc/[pid]/stat
unsigned long parse_cpu_time(pid_t pid)
{
    char filename[256];
    snprintf(filename, sizeof(filename), "/proc/%d/stat", pid);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening /proc/pid/stat");
        exit(EXIT_FAILURE);
    }

    // Read the contents of the stat file
    char line[1024];
    fgets(line, sizeof(line), file);
    fclose(file);

    // Tokenize the line and extract the CPU time
    char *token = strtok(line, " ");
    for (int i = 0; i < 13; ++i) {
        token = strtok(NULL, " "); // Skip to the 14th token (utime)
    }
    unsigned long utime = strtoul(token, NULL, 10);

    token = strtok(NULL, " "); // Skip to the 15th token (stime)
    unsigned long stime = strtoul(token, NULL, 10);

    return utime + stime;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <TOP_PROCESSES_COUNT>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int TOP_PROCESSES_COUNT = atoi(argv[1]);
    if (TOP_PROCESSES_COUNT <= 0) {
        fprintf(stderr, "Invalid value for TOP_PROCESSES_COUNT\n");
        exit(EXIT_FAILURE);
    }

    struct task_struct *task;
    struct top_process_info top_processes[TOP_PROCESSES_COUNT];
    int i;

    // Initialize top processes info
    for (int i = 0; i < TOP_PROCESSES_COUNT; ++i) {
        top_processes[i].pid = -1;
        top_processes[i].cpu_time = 0;
    }

    DIR *dir;
    struct dirent *entry;
    dir = opendir("/proc");
    if (dir == NULL) {
        perror("Error opening /proc directory");
        exit(EXIT_FAILURE);
    }

    // Read /proc directory to find processes
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            pid_t pid = atoi(entry->d_name);
            if (pid != 0) {
                unsigned long cpu_time = parse_cpu_time(pid);

                // Check if this process has higher CPU time than any of the top processes
                for (int i = 0; i < TOP_PROCESSES_COUNT; ++i) {
                    if (cpu_time > top_processes[i].cpu_time) {
                        // Shift other processes down
                        memmove(&top_processes[i + 1], &top_processes[i], (TOP_PROCESSES_COUNT - i - 1) * sizeof(struct top_process_info));

                        // Store new top process
                        top_processes[i].pid = pid;
                        top_processes[i].cpu_time = cpu_time;
                        break;
                    }
                }
            }
        }
    }

    closedir(dir);

    for (i = 0; i < TOP_PROCESSES_COUNT; ++i) {
        int file_descriptor;
        int pid = top_processes[i].pid;

        file_descriptor = open("/proc/cpu_profiler/log", O_RDWR); // Open the file /proc/mp1/log

        write(file_descriptor, &pid, 4); // Write the PID to the file
    }

    // This loop simulates a process running indefinitely. The bash script will kill the process at the end of the time period.
    while (1) {
    };

    return 0;
}
