#!/bin/bash
nekoc rng.neko
# rng-wrapper.sh
RANDOM_NUM=$(( RANDOM % 10 ))  # 0-9
neko rng $RANDOM_NUM
