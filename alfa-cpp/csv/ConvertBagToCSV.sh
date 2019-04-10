#!/bin/bash

# Retrieve the bag file name without the extension
filename=$(basename -- "$1")
filename="${filename%.*}"

rostopic list -b $1 | while read -r topic ; do
    # Replace all '/' with '-'
    csvname="$filename""${topic////-}".csv

    # Convert the topic to CSV
    rostopic echo "$topic" -b "$1" -p > "$csvname"
done
