# Script file for scomm
# testscript.sc
#
# DelaySeconds(), DelayMilliseconds(), and DelayMicroseconds() all accept a single
# parameter, and will delay for the amount of provided time

# The Write() command takes a single quotation mark delimited string that wil be written to the
# connected serial port

# The Loop() command takes a single argument and will loop all of the commands in the curly bracket delimiters
# If Loop() is not given an integer, it will loop forever

# The FlushRX() command flushes the Serial Port receive line
# The FlushTX() command flushes the Serial Port transit line
# The FlushRXTX() and FlushTXRX() commands both flush the Serial Port receive and transmit lines

# Any line began with a pound sign will be ignored

# Any line that has invalid syntax will be ignored, and a warning will be output to stdout

Write("{dwrite:18:0}")
DelayMilliseconds(50)
Read()
Write("{dwrite:19:0}")
DelayMilliseconds(50)
Read()
Write("{dwrite:20:0}")
DelayMilliseconds(50)
Read()
Write("{dwrite:21:0}")
DelayMilliseconds(50)
Read()
Write("{dwrite:22:0}")
DelayMilliseconds(50)
Read()
Write("{caninit}")
DelayMilliseconds(200)
Read()
Write("{canlup:1}")
DelayMilliseconds(50)
Read()
DelayMilliseconds(500)
Write("{addcanmask:0x79A"})
DelayMilliseconds(50)
Read()
Write("{canwriteo:0x792:0x03:0x22:0xFD:0x15:0x00:0x00:0x00:0x00}"})
