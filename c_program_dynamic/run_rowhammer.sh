#!/bin/bash

gcc ./source/rowhammer.c -o ./source/rowhammer

rowhammer="./source/rowhammer" 

echo "[start rowhammer]"
$rowhammer

while true
do
    if [ $? -eq 0 ]; then
        echo "[start rowhammer]"
        $rowhammer
    else
        continue
    fi
done