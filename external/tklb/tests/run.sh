#!/bin/bash

unameOut="$(uname -s)"
case "${unameOut}" in
	Linux*)     executable=./a.out;;
	Darwin*)    executable=./a.out;;
	CYGWIN*)    executable=./a.exe;;
	MINGW*)     executable=./a.exe;;
	*)          executable=./a.out
esac

source_file=$1

exit_code=0

function test {
	if $1 $source_file >> /dev/null 2>&1 ; then
		$executable
		result=$?
		if [[ $result -eq 0 ]] ; then
			echo "Passed: ${1} ${source_file}"
		else
			echo -e "\e[31mError: Test failed for ${source_file} ${1}. Returned ${result}\e[0m"
			exit_code=$result
		fi
		rm $executable
	else
		echo -e "\e[31m${1} Error: Failed to compile ${source_file}\e[0m"
		exit 222
	fi
}

test "g++ -O2 -march=native"
test "g++ -O2 -march=native -DTKLB_NO_SIMD"
test "g++ -O2 -march=native -DTKLB_SAMPLE_FLOAT"
test "g++ -O2 -march=native -DTKLB_SAMPLE_FLOAT -DTKLB_NO_SIMD"
test "clang++ -march=native -Ofast"
test "clang++ -march=native -Ofast -DTKLB_NO_SIMD"
test "clang++ -march=native -Ofast -DTKLB_SAMPLE_FLOAT"
test "clang++ -march=native -Ofast -DTKLB_SAMPLE_FLOAT -DTKLB_NO_SIMD"

# TODO figure out how to get into dev shell for windows
if [ "$executable" == "./a.exe" ]; then
	cmd /C "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
	test "cl /O2 /arch:AVX"
	test "cl /O2 /arch:AVX /DTKLB_NO_SIMD"
	test "cl /O2 /arch:AVX /DTKLB_SAMPLE_FLOAT"
fi

exit $exit_code
