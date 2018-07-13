#!/bin/bash
set -e

DST=./papers

[ ! -d "$DST" ] && mkdir -p "$DST"

SEED=$RANDOM
./criss.py --seed "$SEED" --css-embed --serialize "$DST/$SEED.pickle" > "$DST/$SEED.html" 
chromium --headless --disable-gpu --print-to-pdf="$DST/$SEED.pdf" "$DST/$SEED.html"

xdg-open "$DST/$SEED.pdf"

if [ "$1" != "--dry-run" ]; then
  read
  lp "$DST/$SEED.pdf" -P 1
fi
