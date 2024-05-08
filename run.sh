make
sudo dmesg -C
sudo insmod cpu_profiler.ko
cat /proc/cpu_profiler/log
./userapp &
./userapp &
./userapp &
./userapp &
./userapp &
echo "Sleeping 5 seconds"
sleep 6
echo "Reading the CPU log file"
cat /proc/cpu_profiler/log
echo "Sleeping 5 seconds"
sleep 6
echo "Reading the CPU log file"
cat /proc/cpu_profiler/log
echo "Sleeping 5 seconds"
sleep 6
echo "Reading the CPU log file"
cat /proc/cpu_profiler/log
sudo rmmod cpu_profiler
pkill userapp
sudo dmesg
