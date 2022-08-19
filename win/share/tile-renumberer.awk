# usage: awk -f tile-renumberer.awk [monsters|objects|other].txt > new[monsters|objects|other].txt

BEGIN {
  x = 0;
}

/tile [0-9][0-9]*/ {
  sub(/[0-9][0-9]*/, x);
  x += 1;
}

{
  print $0;
}