#!/bin/sh

# command CppFile OutputFile


if [ $# = 2 ]; then
	OutputFile=$2
else
	OutputFile="${1%.*}"
fi


g++ -O3 -I/home/ycc/projects/streamit-src-2.1.1/library/cluster -c -o "$OutputFile".o $1
g++ -O3 -o "$OutputFile".out "$OutputFile".o -L/home/ycc/projects/streamit-src-2.1.1/library/cluster -lpthread -lcluster -lstdc++ -lpapi