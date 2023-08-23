#!/bin/bash

# check if 2 arguments are sent
if [ $# -eq 2 ]
then
	# remove file name from path
	mkdir -p $(dirname $1)
	# create the file
	touch $1
	# add string
	echo $2 > $1
	echo "File created successfully!"
	exit 0	
else
	echo "Please enter 2 arguments!"
	exit 1
fi
