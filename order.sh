#!/bin/bash

awk -f win/share/tile-renumberer.awk win/share/objects.txt > win/share/otmp.txt
rm -v win/share/objects.txt
mv -v win/share/otmp.txt win/share/objects.txt
