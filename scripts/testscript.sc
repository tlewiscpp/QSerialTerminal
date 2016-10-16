# Script file for scomm
# testscript.sc
#
# SetDelaySeconds(), SetDelayMilliseconds(), and SetDelayMicroseconds() all accept a single
# parameter, and will delay for the amount of provided time

# The Write() command takes a single quotation mark delimited string that wil be written to the
# connected serial port

# Any line began with a pound sign will be ignored

# Any line that has invalid syntax will be ignored, and a warning will be output to stdout

DelayMilliseconds(500)
Write("{dwrite:19:0}")
DelayMilliseconds(25)
Read()
Write("{dwrite:20:0}")
DelayMilliseconds(25)
Read()
Write("{dwrite:21:0}")
DelayMilliseconds(25)
Read()
Write("{dwrite:22:0}")
DelayMilliseconds(25)
Read()
Write("{caninit}")
DelayMilliseconds(100)
Read()
Write("{canlup:1}")
DelayMilliseconds(25)
Read()
DelayMilliseconds(25)
