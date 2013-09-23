#!/bin/sh

FileList="$PWD/*"
for f in $FileList
do
	suffix=${f##*.}
	if [ $suffix = "str" -o $suffix = "sh" ]; then
		continue
	else
		rm -f $f
	fi
done