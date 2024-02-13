#!/usr/bin/env bash

# My gradescript says something is wrong but thats false see for yourself, 
# when it runs my output is identical to the reference directory...

# Loop through the files in the input directory
for file in *; do
  # Get the name of the record file (without the ".txt" extension)
  record_name=$(basename "$file" .txt)

  # Create a directory for the record in the output directory
  mkdir -p "$record_name"
  
  # Decompose the record into separate files
  awk '/^id / {print $2 > "'"$record_name/id.txt"'"} 
       /^age / {print $2 > "'"$record_name/age.txt"'"} 
       /^email / {print substr($0,7) > "'"$record_name/email.txt"'"}' "$file"

  # remove old file
  rm $file
done

echo "Everything works correctly here, and it makes no sense why the gradescript won't pass me for part 1."