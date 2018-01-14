#!/bin/bash
echo "Testing parser"

if [ -e "./text" ]
then
	rm ./text
fi

echo "Testing all operators on integers"

../lindaCommunicator -o "( 0, 2, 3, 4, 5, 6)"
../lindaCommunicator -o "( 1, 3, 2, 4, 5, 6)"
../lindaCommunicator -o "( 1, 2, 3, 5, 5, 6)"
../lindaCommunicator -o "( 1, 2, 3, 4, 6, 6)"
../lindaCommunicator -i "( >0, <3, >=3, <=4, ==5, *)" -t 1000

echo "Empty linda returned"

echo "Adding right linda"

../lindaCommunicator -o "( 1, 2, 3, 4, 5, 6)"

echo "Trying to read it"
../lindaCommunicator -i "( >0, <3, >=3, <=4, ==5, *)"


echo "Testing all operators on strings"

../lindaCommunicator -o "( \"0\", \"2\", \"3\", \"4\", \"5\", \"6\")"
../lindaCommunicator -o "( \"1\", \"3\", \"2\", \"4\", \"5\", \"6\")"
../lindaCommunicator -o "( \"1\", \"2\", \"3\", \"5\", \"5\", \"6\")"
../lindaCommunicator -o "( \"1\", \"2\", \"3\", \"4\", \"6\", \"6\")"
../lindaCommunicator -i "( >\"0\", <\"3\", >=\"3\", <=\"4\", ==\"5\", \"*\")" -t 1000

echo "Empty linda returned"

echo "Adding right linda"

../lindaCommunicator -o "( \"1\", \"2\", \"3\", \"4\", \"5\", \"6\")"

echo "Trying to read it"
../lindaCommunicator -i "( >\"0\", <\"3\", >=\"3\", <=\"4\", ==\"5\", \"***6***\")"


