#!/bin/bash

default_vl=("1")

# 检查是否有参数输入，如果没有则使用默认值
if [ $# -eq 0 ]; then
    echo "No VL values provided. Running default VL=$default_vl."
    vl_values=("${default_vl[@]}")
else
    vl_values=("$@")

fi

function run_command {
    local command=$1
    local output_name=$2

    echo "Running command: $command at $(date)" | tee -a $LOGFILE
    timeout 86400 $command 2>&1 | tee -a $LOGFILE

    local command_status=${PIPESTATUS[0]}
    if [[ $command_status -eq 124 ]]; then
        echo "Command '$command' exceeded time limit and was terminated at $(date)" | tee -a $LOGFILE
    else
        echo "Command '$command' completed at $(date)" | tee -a $LOGFILE

        # 复制 m5out/stats.txt 到输出文件夹并重命名
        cp m5out/stats.txt $OUTPUT_DIR/stats-$output_name.txt
        echo "Copied stats to $OUTPUT_DIR/stats-$output_name.txt" >> $LOGFILE
    fi
    echo "------------------------------" | tee -a $LOGFILE
}

# 定义日志文件
LOGFILE="1-logfile.txt"

# 定义输出文件夹
# OUTPUT_DIR="gem5output/TimingSimple"
OUTPUT_DIR="gem5output/O3"

# 创建输出文件夹，如果不存在
mkdir -p $OUTPUT_DIR

# 记录脚本开始时间
echo "Script started at $(date)" | tee -a $LOGFILE

# 文件数组（按照_n, _sve, _sme的顺序）
files=(
    # "128size-128kb-n.py"
    "128size-128kb-sve.py"
    "128size-128kb-sme.py"
    # "128size-256kb-n.py"
    "128size-256kb-sve.py"
    "128size-256kb-sme.py"
    # "128size-512kb-n.py"
    "128size-512kb-sve.py"
    "128size-512kb-sme.py"

    # "256size-128kb-n.py"
    "256size-128kb-sve.py"
    "256size-128kb-sme.py"
    # "256size-256kb-n.py"
    "256size-256kb-sve.py"
    "256size-256kb-sme.py"
    # "256size-512kb-n.py"
    "256size-512kb-sve.py"
    "256size-512kb-sme.py"

    # "384size-128kb-n.py"
    "384size-128kb-sve.py"
    "384size-128kb-sme.py"
    # "384size-256kb-n.py"
    "384size-256kb-sve.py"
    "384size-256kb-sme.py"
    # "384size-512kb-n.py"
    "384size-512kb-sve.py"
    "384size-512kb-sme.py"

    # "512size-128kb-n.py"
    "512size-128kb-sve.py"
    "512size-128kb-sme.py"
    # "512size-256kb-n.py"
    "512size-256kb-sve.py"
    "512size-256kb-sme.py"
    # "512size-512kb-n.py"
    "512size-512kb-sve.py"
    "512size-512kb-sme.py"
)

# 遍历并执行每个文件
for file in "${files[@]}"; do
    base_name=$(basename $file .py)

    # 检查文件后缀并设置命令
    if [[ $file == *-n.py ]]; then
        cmd="build/ARM/gem5.opt configs/withcache/n/$file"
        run_command "$cmd" "$base_name"
    else
        for vl in "${vl_values[@]}"; do
            if [[ $file == *-sve.py ]]; then
                cmd="build/ARM/gem5.opt configs/withcache/sve/$file --vl=$vl"
            elif [[ $file == *-sme.py ]]; then
                cmd="build/ARM/gem5.opt configs/withcache/sme/$file --vl=$vl"
            fi
            run_command "$cmd" "$base_name-vl$vl"
        done
    fi
done

# 记录脚本结束时间
echo "Script finished at $(date)" | tee -a $LOGFILE