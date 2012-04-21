#include <cassert>

#include "AnuRandom.hpp"

using namespace std;


//
// API's PIMPL implementation
//

class AnuRandom::Impl
{
public:
  Impl(string&& host, string&& path)
  {
    // TODO
  }

  void read(uint8_t *buf, size_t size)
  {
    // TODO
  }
};



//
// API implementation
//

AnuRandom::AnuRandom(std::string host, std::string path):
  pimpl_{ new Impl{ move(host), move(path) } }
{
}

AnuRandom::~AnuRandom(void)
{
  // NOTE: this ensures proper destruction of implementation object
}

void AnuRandom::read(uint8_t *buf, size_t size)
{
  assert( pimpl_.get()!=nullptr );
  pimpl_->read(buf, size);
}
