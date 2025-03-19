#!/bin/bash

path="/gpfs1/share/class2/chr21.fa"
total_length=$(wc -c $path | awk '{print $1}')
A_counts=$(grep -io 'a' $path | wc -l)
echo "The length of Chromosome21 is $total_length bp."
echo "The counts of Adenine in Chromosome21 is $A_counts."
