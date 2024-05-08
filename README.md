# CPU Profiler - Linux Module

## Overview

CPU Profiler is a kernel module designed to profile CPU time usage of user-space processes in a Linux environment. This README provides comprehensive documentation on the design, implementation, and usage of the module.

## Design and Implementation

### API

The module provides a simple API for user-space application registration. The `write` function is utilized for registration, allowing applications to send an integer representing the process ID (PID). Internally, this PID is handled to create a new element in a list. The `copy_from_user` method is employed to ensure secure and robust memory management.

### Timer Interrupt Handling

During timer interruptions, the module performs three main tasks:

1. Checking if the process list is empty.
2. Enqueuing work.
3. Retriggering the timer.

This design ensures that the routine remains concise and exhibits good performance. Anomalies, such as processes no longer existing, are detected and handled during the enqueue work phase to update CPU time accordingly.

### Timer Setup

The module utilizes a method to convert milliseconds to jiffies for setting up the timer. This approach simplifies timer configuration and ensures accuracy without the need for manual jiffies conversions. Leveraging a method implemented within the kernel guarantees optimal performance.

### CPU Time Update

The function responsible for updating CPU time utilizes `cputime_to_jiffies` and `jiffies_to_msec` methods for conversion, ultimately setting the CPU time in milliseconds in the internal list.

### Efficiency and Robustness Mechanisms

Several mechanisms are implemented to enhance efficiency and robustness:

- Macros prevent the module from triggering the timer if no process is registered. Once a process is registered, the timer is set, and if the list becomes empty, the timer is no longer retriggered.
- The absence of `goto` statements is attributed to the module's simplicity and reduced functionality. However, future enhancements may incorporate `goto` statements to manage increased complexity while prioritizing performance.
- Each allocation is properly deallocated to mitigate memory leaks, ensuring memory management integrity.

## Usage

To execute the program, follow these steps:

1. Compile the program using the provided `Makefile`.
2. Load the module using `insmod`.
3. Execute the test script `test.sh`, which performs the following actions:
   - Clears the DMESG log.
   - Cleans and recompiles both the module and the user application.
   - Loads the module.
   - Runs a test scenario involving multiple user applications.
   - Unloads the module and terminates user applications.
   - Displays the DMESG log for internal checkpoints.

## Test and Output Examples

### Normal Execution

The provided test script demonstrates a scenario of normal execution, where multiple user applications are registered, CPU time is profiled, and output is retrieved using `cat` commands.

### Removing All User Applications in the Middle

This test scenario simulates the removal of all user applications during execution. The module detects the absence of processes and removes them from the list, ensuring proper cleanup and integrity.

### Removing Some User Applications in the Middle

In this test case, some user applications are removed during execution, showcasing the module's ability to dynamically adapt to changes in process status and maintain accurate CPU time profiling.

## Conclusion

CPU Profile offers a robust and efficient solution for CPU time profiling in Linux environments. With its simple yet effective design, the module ensures accurate profiling while minimizing overhead. Future improvements may focus on enhancing functionality and scalability to meet evolving requirements.

For further information or inquiries, please refer to the project's GitHub repository or contact the developers directly.

---
