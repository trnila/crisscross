#!/bin/bash
set -e

PARAMS="--rows 35 --cols 33 --max-words 100 -a 15"
DST=./papers

make

[ ! -d "$DST" ] && mkdir -p "$DST"

SEED=$RANDOM
./crisscross --format html --seed "$SEED" $PARAMS > "$DST/$SEED.html" 
./crisscross --format html --seed "$SEED" --print-solution $PARAMS > "$DST/${SEED}_solution.html" 
chromium --headless --disable-gpu --print-to-pdf="$DST/$SEED.pdf" "$DST/$SEED.html"

xdg-open "$DST/$SEED.pdf"
read
lp "$DST/$SEED.pdf"
