#!/bin/bash
echo "Testing random file reading"

if [ -e "./text" ]
then
	rm ./text
fi

for i in {1..50}
do
	../lindaCommunicator -o "( $i )"
done

echo "File filled"
echo "Trying to peek lines"

for i in {1..50}
do
	../lindaCommunicator -r "( * )"
done

echo "Trying to read all file"

for i in {1..50}
do
	../lindaCommunicator -i "( * )"
done

