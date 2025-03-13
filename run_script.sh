#!/bin/bash

# test files
test_files=("a1.in" "a2.in" "a3.in" "a4.in" "a5.in" "a6.in" "a7.in" "a8.in")

# Loop through the test files and run the scanner on each one
for test_file in "${test_files[@]}"; do
    echo "Parsing file: $test_file"
    ./proj2 "$test_file" 
    echo "---------------------------"
done
