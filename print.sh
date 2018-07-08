#!/bin/bash
set -e

PARAMS="--css-embed"
DST=./papers

[ ! -d "$DST" ] && mkdir -p "$DST"

SEED=$RANDOM
./criss.py --seed "$SEED" $PARAMS > "$DST/$SEED.html" 
chromium --headless --disable-gpu --print-to-pdf="$DST/$SEED.pdf" "$DST/$SEED.html"

xdg-open "$DST/$SEED.pdf"

if [ "$1" != "--dry-run" ]; then
  read
  lp "$DST/$SEED.pdf"
fi
