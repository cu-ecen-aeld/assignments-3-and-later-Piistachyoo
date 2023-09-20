#!/bin/sh

./aesdchar_unload
make clean
make
./aesdchar_load