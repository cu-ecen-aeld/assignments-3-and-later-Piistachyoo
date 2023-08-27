#!/bin/sh

# check if arguments is not equal to 2
if [ $# -eq 2 ]
then
	cd $1 
	# check if the path is found
	if [ $? -eq 1 ]
	then
		echo "Please enter a valid directory"
		exit 1
	else
		# get number of lines
		X=$( ls $1 | wc -l )
		#get number of matches
		Y=$( grep -r $2 $1 | wc -l )
		echo "The number of files are $X and the number of matching lines are $Y"
		exit 0
	fi
else
	echo "Please enter 2 arguments"
	exit 1
fi
