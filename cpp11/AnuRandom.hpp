#ifndef INCLUDE_ANURANDOM_HPP_FILE
#define INCLUDE_ANURANDOM_HPP_FILE

#include <memory>
#include <string>
#include <utility>
#include <stdexcept>
#include <cstdint>


class AnuRandom
{
public:
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


  explicit AnuRandom(std::string host="150.203.48.55",
                     std::string path="RawHex.php");
  ~AnuRandom(void);

  template<size_t N>
  void read(uint8_t (&buf)[N])
  {
    read(buf, N);
  }

  void read(uint8_t *buf, size_t size);

private:
  class Impl;
  std::unique_ptr<Impl> pimpl_;
};

#endif
