#!/usr/bin/env bash

# Create a variable for combined sequence
sequence=""
declare -a ARRAY=()

# Loop through all sample files
for file in *.sample; do
  # Extract the sequence position and data using awk
  data=$(awk 'NR==3 {print $3}' "$file")
  pos=$(awk 'NR==1 {print $3}' "$file")
  # Extract the sequence data using cut and remove newlines using tr
  seq=$(echo "$data" | cut -d ' ' -f 2- | tr -d '\n')
  ARRAY[pos]=$seq
  # Add the sequence data to the combined sequence variable
done

# loop through sorted array and create combine sequences
for i in ${ARRAY[@]}
do
sequence+="$i"
done

# remove newline from sequence 
sequence="$(echo "$sequence"|tr -d '\n')"

# Output the combined sequence to a file called sequence.txt
echo "$sequence" | tr -d '\n' >  sequence.txt

# Use fold and awk to count the number of each base pair and output to pairs.txt
fold -w 1 sequence.txt | awk '
  {count[$1]++}
  END {print "A " count["A"] "\nT " count["T"] "\nC " count["C"] "\nG " count["G"]}' > pairs.txt

# Use fold and awk to count the number of each codon and output to codons.txt
fold -w 3 sequence.txt | awk '
  {
    codons[$0]++
    total++
  }
  END {
    for (codon in codons) {
      percent = (codons[codon] / total) * 100
      printf "%s %.6f%%\n", codon, percent
    }
  }' | sort > codons.txt
