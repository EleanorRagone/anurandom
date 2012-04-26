#!/bin/sh
# halt on any error
set -e
# setup
LINK="http://150.203.48.55/RawHex.php"
TEMP="`mktemp /tmp/.anurandom_shell_tmp.XXXXXXXXX`"
# download file
wget -O "$TEMP" -q "$LINK" || { RET=$? ; echo "downloading '$LINK' failed" ; exit $? ; }
# get the hex stream
grep '[a-fA-F0-9]\{64,\}' "$TEMP" | sed 's:\([a-fA-F0-9]\+\).*:\1:g'
# cleanup
rm "$TEMP"
