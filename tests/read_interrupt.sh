#!/bin/bash

if [ -e "./text" ]
then
	rm ./text
fi

echo "Trying to read linda that shows halfway through"

../lindaCommunicator -r "( * )" -t 20000 &

sleep 5s

echo "Adding missing linda"
../lindaCommunicator -o "( 1 )"
