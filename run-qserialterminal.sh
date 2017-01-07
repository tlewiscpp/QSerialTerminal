#!/bin/bash

#if [[ "$EUID" -eq "0" ]]; then
    abconnect.sh &
#fi

QSerialTerminal >> /tmp/$(date +%s)-$$-qserialterminal.log

return $?
