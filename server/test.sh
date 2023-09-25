target=localhost
port=9000
rc=0

function send_socket_string
{
	string=$1
	result_file=$2
	echo ${string} | nc ${target} ${port} -w 1 > /dev/null
}

function test_send_socket_string
{
	string=$1
	prev_file=$2
	new_file=`tempfile`
	expected_file=`tempfile`
	send_socket_string ${string} ${new_file}
	cp ${prev_file} ${expected_file}
	echo ${string} >> ${expected_file}
	diff ${expected_file} ${new_file} > /dev/null
	if [ $? -ne 0 ]; then
		echo "Differences found after sending ${string} to ${target} on port ${port}"
		echo "Expected contents to match:"
		cat ${expected_file}
		echo "But found contents:"
		cat ${new_file}
		echo "With differences"
		diff -u ${expected_file} ${new_file}
		echo "Test complete with failure"
		#exit 1
	else
		cp ${expected_file} ${prev_file}
		rm ${new_file}
		rm ${expected_file}
	fi
}

#send_socket_string "swrite1" 
#send_socket_string "swrite2" 
#send_socket_string "swrite3"

#cat /dev/aesdchar

echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@"

send_socket_string "AESDCHAR_IOCSEEKTO:0,2"  ${seek_result}

#send_socket_string "Omar Yamany"  ${seek_result}
cat /dev/aesdchar
