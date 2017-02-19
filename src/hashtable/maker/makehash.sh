#!/bin/sh

gperf --includes --readonly-tables --pic --struct-type --slot-name=word words.txt > sentiment.h
