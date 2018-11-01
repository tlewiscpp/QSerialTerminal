#!/bin/bash

if [[ "$#" -eq "0" ]]; then
    echo "Usage: $0 [ExistingBuildDirectory]"
    exit 1
fi

if [[ ! -f "$1/install_manifest.txt" ]]; then
    echo "$1 does not appear to be a completed build directory"
    echo "Rerun with a build directory that contains a file named \"install_manifest.txt\""
    exit 1
fi

for file in $(cat "$1/install_manifest.txt"); do
    echo "Removing $file"
    if [[ -f "$file" ]]; then
        sudo rm $file
    fi
done

