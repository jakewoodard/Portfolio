i=1
while [ $i -le 100 ]; do
  echo `bin/race1`                    # This line  runs bin/race1 and prints both lines on a single line.
  i=$(($i+1))
done
