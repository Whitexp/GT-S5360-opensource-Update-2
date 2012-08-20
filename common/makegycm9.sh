CPU_CORES="cat /proc/cpuinfo| grep processor| wc -c"

make clean
make bcm21553_totoro_05_cm9_defconfig
make  -j5
