#!/bin/bash

# Check if exactly two arguments are provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <input_file> <output_file>"
    exit 1
fi

input_file="$1"
output_file="$2"
temp_file="$output_file.tmp"

# Check if input file exists
if [ ! -f "$input_file" ]; then
    echo "Error: Input file '$input_file' not found."
    exit 1
fi

# Extract unique cities from input file
unique_cities=$(awk -F ', ' 'NR>1{print $3}' "$input_file" | sort -u)

# Print unique cities to output file
echo "------------------" >> "$output_file"
echo "Unique cities in the given data file: " >> "$output_file"
echo "$unique_cities" >> "$output_file"
echo "------------------" >> "$output_file"

# Find top 3 individuals with highest salary
top_3=$(awk 'NR>1{print $0}'  "$input_file"| sort -t',' -k4r -k1| head -n 3)

# Print top 3 individuals to output file
echo "Details of top 3 individuals with the highest salary: " >> "$output_file"
echo "$top_3" | awk -F ', ' '{print $0}' >> "$output_file"
echo "------------------" >> "$output_file"

# Compute average salary for each city
city_avg_salary=$(awk -f city_avg_salary.awk "$input_file")

# Print average salary of each city to output file
echo "Details of average salary of each city: " >> "$output_file"
echo "$city_avg_salary" >> "$output_file"
echo "------------------" >> "$output_file"

# Compute average salary
avg_salary=$(awk -f avg_salary.awk "$input_file")

# # Identify individuals with salary above average
above_avg=$(awk -v avg="$avg_salary" -f above_avg.awk "$input_file")

# Print individuals with salary above average to output file
echo "Details of individuals with a salary above the overall average salary: " >> "$output_file"
echo "$above_avg" >> "$output_file"
echo "------------------" >> "$output_file"