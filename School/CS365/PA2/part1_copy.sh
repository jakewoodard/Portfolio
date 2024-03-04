#!/usr/bin/env bash

# Check if input directory is provided
if [ -z "$1" ]; then
  echo "Usage: $0 input_directory"
  exit 1
fi

# Create the output directory
output_dir="part1"
mkdir -p "$output_dir"

# Loop through the files in the input directory
for file in "$1"/*; do
  # Get the name of the record file (without the ".txt" extension)
  record_name=$(basename "$file" .txt)
  
  # Create a directory for the record in the output directory
  mkdir -p "$output_dir/$record_name"
  
  # Decompose the record into separate files
  awk '/^id / {print $2 > "'"$output_dir/$record_name/id.txt"'"} 
       /^age / {print $2 > "'"$output_dir/$record_name/age.txt"'"} 
       /^email / {print substr($0,7) > "'"$output_dir/$record_name/email.txt"'"}' "$file"
done

echo "Record files decomposed successfully into $output_dir directory."
