#!/bin/bash

# Set input file name
filename="cars.csv"

#!/bin/bash

column_number=6
make_number=1


max_value=-99999999999999999999

while read line
do
    column_value=$(echo $line | cut -d ',' -f $column_number)
    make_value=$(echo $line | cut -d ',' -f $make_number)
    if (( $(echo "$column_value > $max_value" |bc -l) ))
    then
        max_value=$column_value
        max_make=$make_value
    fi
done < $filename

echo "${max_make} ${max_value}" >> answer.txt