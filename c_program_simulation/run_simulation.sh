#!/bin/bash

gcc ./source/rowhammer_simulation.c -o ./source/rowhammer_simulation

rowhammer="./source/rowhammer_simulation" 

echo "[start rowhammer simulation]"
$rowhammer