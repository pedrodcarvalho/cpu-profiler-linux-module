#!/bin/bash

set -e

make
sudo dmesg -C
sudo insmod cpu_profiler.ko
cat /proc/cpu_profiler/log

# Run userapp in the background and store its PID
./userapp 10 &
USERAPP_PID=$!

# Check if userapp is still running
if ! ps -p $USERAPP_PID > /dev/null; then
    echo "Error: userapp failed to start"
    sudo rmmod cpu_profiler
    pkill userapp
    sudo dmesg
    exit 1
fi

echo "Reading the CPU log file"
cat /proc/cpu_profiler/log

echo "Sleeping 5 seconds"
sleep 6

if ! ps -p $USERAPP_PID > /dev/null; then
    echo "Error: userapp terminated prematurely"
    exit 1
fi

echo "Reading the CPU log file"
cat /proc/cpu_profiler/log

echo "Sleeping 5 seconds"
sleep 6

if ! ps -p $USERAPP_PID > /dev/null; then
    echo "Error: userapp terminated prematurely"
    exit 1
fi

echo "Reading the CPU log file"
cat /proc/cpu_profiler/log

sudo rmmod cpu_profiler
pkill userapp
sudo dmesg
