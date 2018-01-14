#!/bin/bash
echo "Testing concurrent reading"

if [ -e "./text" ]
then
	rm ./text
fi

../lindaCommunicator -o "( 1 )"

echo "Setting up readers"

for i in {1..100}
do
	../lindaCommunicator -i "( * )" -t 30000 &
done

echo "Filling file"

sleep 5

for i in {1..50}
do
	../lindaCommunicator -o "( $i )" -t 30000
done

sleep 5
echo "Filling file second time"

for i in {1..50}
do
	../lindaCommunicator -o "( $i )" -t 30000
done
