#!/bin/bash

if [ -e "./text" ]
then
	rm ./text
fi

echo "Trying to read same linda twice"
../lindaCommunicator -o "( 1 )"


../lindaCommunicator -i "( * )"
../lindaCommunicator -i "( * )"

