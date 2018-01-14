#!/bin/bash

if [ -e "./text" ]
then
	rm ./text
fi

echo "Trying some wrong lindas"

../lindaCommunicator -o "( 1"
../lindaCommunicator -o "1"
../lindaCommunicator -o "( * )"
../lindaCommunicator -o "( 1, )"
../lindaCommunicator -o "( >1 )"
../lindaCommunicator -o "( >\"1\" )"
../lindaCommunicator -r "( <>\"1\" )"



