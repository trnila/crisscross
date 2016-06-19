#!/usr/bin/env bash
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"


for in in $(find "$DIR" -name "*.in"); do
	out=$(basename "$in" .in).out
	echo $out

	"$DIR"/../formatter < "$in" | diff - "$DIR/$out"
	if [ ! $? -eq 0 ]; then
		echo "failed"
		"$DIR"/../formatter < "$in"
	fi

done