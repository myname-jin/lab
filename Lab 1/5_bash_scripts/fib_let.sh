#!/bin/bash
count=10
numbera=0
numberb=1
echo "피보나치 수열 (let 버전):"
for ((i=0; i<$count; i++))
do
    echo "$numbera"
    let fn=numbera+numberb
    let numbera=numberb
    let numberb=fn
done