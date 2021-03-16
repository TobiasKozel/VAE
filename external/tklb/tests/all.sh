#!/bin/bash

FAILED=false

for f in ./*.cpp
do
	./run.sh $f
	if [[ $? -ne 0 ]] ; then
		FAILED=true
	fi
done

if ($FAILED); then
	echo -e "\e[31mSome tests failed!\e[0m"
	exit 1
else
	echo -e "\e[32mAll tests passed!\e[0m"
fi
