#!/bin/bash
echo "Testing filling test file"

if [ -e "./text" ]
then
	rm ./text
fi

../lindaCommunicator -o "( 1 )"

for i in {1..49}
do
	../lindaCommunicator -o "( $i )"
done

echo "File filled"
echo "Trying one more time"
../lindaCommunicator -o "( 1 )" &

sleep 5

echo "No success until read"
../lindaCommunicator -i "( * )"

