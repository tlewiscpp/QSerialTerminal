# Script file for scomm
# testscript.sc
#
# SetDelaySeconds(), SetDelayMilliseconds(), and SetDelayMicroseconds() all accept a single
# parameter, and will delay for the amount of provided time

# The Write() command takes a single quotation mark delimited string that wil be written to the
# connected serial port

# Any line began with a pound sign will be ignored

# Any line that has invalid syntax will be ignored, and a warning will be output to stdout

Loop(6) {
    Write("{dwrite:13:1}")
    DelayMilliseconds(1000)
    Read()
    Write("{dwrite:13:0}")
    DelayMilliseconds(1000)
    Read()
    FlushRXTX()
}
