The full resource is on GitHub: https://github.com/Jzx-china/Scaling-GEMM-on-the-ARM-Scalable-Vector-Matrix-Extensions.git. You can run the 1-commands.sh on gem5 platform. This shell contains severl commands and can run several configurations without hand-checking. The logfile will be stored in 1-logfile.txt and outputs will be stored in gem5outputs automatically.

The file configs/withcache contains all the configuration used.
The file running contains all GEMM codes and binaries.
You can use the on gem5(https://github.com/gem5/gem5) by yourself or directly use there
The recommanded environment is official Docker container ubuntu-22.04_all-dependencies provided by gem5