#!/bin/bash

QSerialTerminal >> /tmp/$(date +%s)-$$-qserialterminal.log

return $?
