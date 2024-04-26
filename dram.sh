#!/bin/bash

g++ -o dram_simulator dram.cpp
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

rows_array=(1024)
cols_array=(1024)
error_rates=(1 5 10 20)
num_accesses=(15000000000 20000000000)
scrub_intervals=(30 60 120 300)
sim_times=(3600)
batches=(300) 
log_file="dram_sim_results3.txt"

echo "Simulation results log" > $log_file
echo "Timestamp: $(date)" >> $log_file
echo "-------------------------------" >> $log_file

for rows in "${rows_array[@]}"; do
    for cols in "${cols_array[@]}"; do
        for error_rate in "${error_rates[@]}"; do
            for accesses in "${num_accesses[@]}"; do
                for scrub in "${scrub_intervals[@]}"; do
                    for sim_time in "${sim_times[@]}"; do
                        for batch in "${batches[@]}"; do
                            echo "Running simulation with $rows rows, $cols cols, $error_rate errors/hour, $accesses accesses, scrub every $scrub seconds, for $sim_time seconds, in $batch batches" | tee -a $log_file
                            ./dram_simulator $rows $cols $error_rate $accesses $scrub $sim_time $batch >> $log_file 2>&1
                            echo "Simulation completed." | tee -a $log_file
                            echo "" | tee -a $log_file
                        done
                    done
                done
            done
        done
    done
done

echo "All simulations completed." | tee -a $log_file
echo "-------------------------------" >> $log_file
echo "End of log" >> $log_file
