#!/usr/bin/env bash
awk -F ',' 'NR==1{for(i=1;i<=NF;i++)if($i=="Horsepower")col=i}NR>1{if($col>max){max=$col;make=$1}}END{printf("%s %.6f\n", make, max) > "answer.txt"}' cars.csv
