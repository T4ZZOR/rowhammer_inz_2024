#!/bin/bash

gcc ./source/rowhammer_dynamic.c -o ./source/rowhammer_dynamic

rowhammer="./source/rowhammer_dynamic" 

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