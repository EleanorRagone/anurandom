#ifndef INCLUDE_ANURANDOM_HPP_FILE
#define INCLUDE_ANURANDOM_HPP_FILE

#include <vector>
#include <memory>
#include <string>
#include <utility>
#include <stdexcept>
#include <cstdint>


class AnuRandom
{
public:
  struct ExceptionParameterError: public std::runtime_error
  {
    explicit ExceptionParameterError(std::string msg):
      std::runtime_error{ std::move(msg) }
    {
    }
  };

  struct ExceptionIO: public std::runtime_error
  {
    explicit ExceptionIO(std::string msg):
      std::runtime_error{ std::move(msg) }
    {
    }
  };

  struct ExceptionParseError: public std::runtime_error
  {
    explicit ExceptionParseError(std::string msg):
      std::runtime_error{ std::move(msg) }
    {
    }
  };

  typedef std::vector<uint8_t> Data;

  explicit AnuRandom(std::string host="150.203.48.55",
                     std::string path="/RawHex.php");
  ~AnuRandom(void);

  void read(Data &out);

  Data read(void)
  {
    Data tmp;
    read(tmp);
    return tmp;
  }

private:
  class Impl;
  std::unique_ptr<Impl> pimpl_;
};

#endif
