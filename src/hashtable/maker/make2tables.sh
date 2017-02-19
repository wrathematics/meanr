#!/bin/sh

gperf --includes --readonly-tables --pic --hash-function-name=poshash --lookup-function-name=in_pos_set --slot-name=word positive.txt > poshash.h
gperf --includes --readonly-tables --pic --hash-function-name=neghash --lookup-function-name=in_neg_set --slot-name=word negative.txt > neghash.h
