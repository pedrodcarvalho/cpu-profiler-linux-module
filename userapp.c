#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int file_descriptor;
    int pid;

    pid = getpid(); // Get the process id

    printf("New process created | PID: %d\n", pid); // Print the PID

    file_descriptor = open("/proc/cpu_profiler/log", O_RDWR); // Open the file /proc/mp1/log

    write(file_descriptor, &pid, 4); // Write the PID to the file

    // This loop simulates a process running indefinitely. The bash script will kill the process at the end of the time period.
    while (1) {
    };

    return 0;
}
