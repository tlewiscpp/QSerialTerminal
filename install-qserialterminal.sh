#!/bin/bash

if [[ "$EUID" != "0" ]]; then
    echo "This script must be run as root"
    exit 1
fi

srcDir=/opt/GitHub/QSerialTerminal
binDir=/opt/QtBuilds/QSerialTerminal/Debug/

ln -s $srcDir/qserialterminal.desktop /usr/share/applications/
ln -s $srcDir/qserialterminal.png /usr/share/pixmaps/
ln -s $srcDir/run-qserialterminal.sh /usr/bin/
ln -s $binDir/QSerialTerminal /usr/bin/
