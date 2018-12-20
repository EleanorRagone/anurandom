import sys

if sys.version_info >= (3,0):
    from urllib.request import urlopen
else:
    from urllib2 import urlopen

class ANURandom:
    BINARY = "BINARY"
    HEX = "HEX"
    CHAR = "CHAR"

    def getRandom(self,type):
        if type == self.BINARY:
            url = 'http://150.203.48.55/RawBin.php'
        elif type == self.HEX:
            url = 'http://150.203.48.55/RawHex.php'
        elif type == self.CHAR:
            url = 'http://150.203.48.55/RawChar.php'

        page = urlopen(url, timeout=5)

        data = page.read()
        num = data.decode().split('"rng"')[1].split('<td>\n')[1].split('</td>')[0]
        return num

    def getBin(self):
        return self.getRandom(self.BINARY)

    def getHex(self):
        return self.getRandom(self.HEX)

    def getChar(self):
        return self.getRandom(self.CHAR)
