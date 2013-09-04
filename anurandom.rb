require 'open-uri'

class ANURandom

  BINARY = "BINARY"
  HEX = "HEX"
  CHAR = "CHAR"

  def getRandom(self, type=BINARY)

    if type == self.BINARY
      url = "http://150.203.48.55/RawBin.php"
    elsif type == self.HEX
      url = 'http://150.203.48.55/RawHex.php'
    elsif type == self.CHAR
      url = 'http://150.203.48.55/RawChar.php'
    end

    page_contents = open(url,) { |f| f.read }

    num = page_contents.split('"rng"')[1].split('<td>\n')[1].split('</td>')[0]

    return num

  end

  def getBin(self)
    getRandom self, BINARY
  end

  def getHex(self)
    getRandom self, HEX
  end

  def getChar(self)
    getRandom self, CHAR
  end

end
