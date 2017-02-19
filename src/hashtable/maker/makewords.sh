#!/bin/sh

# positive 1, negative -1
echo "struct sentwords{ char *word; int score; };" > words.txt
echo "%%" >> words.txt

sed 's/.*/&, 1/' positive.txt >> words.txt
sed 's/.*/&, -1/' negative.txt >> words.txt
