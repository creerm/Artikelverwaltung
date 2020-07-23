#!/bin/bash

gcc -Wall -g *.c `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

if [ $? == 0 ]
then
./a.out daten.csv Lieferungen.csv
fi