#!/bin/bash

# Re-order monsters.txt
awk -f win/share/tile-renumberer.awk win/share/monsters.txt > win/share/mtmp.txt
rm -v win/share/monsters.txt
mv -v win/share/mtmp.txt win/share/monsters.txt

# Re-order objects.txt
awk -f win/share/tile-renumberer.awk win/share/objects.txt > win/share/otmp.txt
rm -v win/share/objects.txt
mv -v win/share/otmp.txt win/share/objects.txt

# Re-order other.txt
awk -f win/share/tile-renumberer.awk win/share/other.txt > win/share/rtmp.txt
rm -v win/share/other.txt
mv -v win/share/rtmp.txt win/share/other.txt


