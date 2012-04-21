#include <deque>
#include <sstream>
#include <cassert>
#include <boost/asio.hpp>

#include "AnuRandom.hpp"

using namespace std;
using boost::asio::ip::tcp;


//
// helper classes
//
namespace
{

// wrapper for network and HTTP protocol
class HttpIo
{
public:
  HttpIo(string&& host, string&& path):
    host_{ move(host) },
    path_{ move(path) }
  {
  }

  std::string readContent(void)
  {
    // do the work
    tcp::socket sock=connectToService();
    // TODO
    return "TODO";
  }

private:
  tcp::socket connectToService(void)
  {
    stringstream log;
    log<<"call history:"<<endl;
    log<<"resolving '"<<host_<<"'..."<<endl;

    try
    {
      tcp::socket             sock(io_);
      tcp::resolver           resolver(io_);
      tcp::resolver::query    query(host_, "http");
      // iterate over all possible resolutions
      for(auto it = resolver.resolve(query); it!=tcp::resolver::iterator(); ++it)
        if( tryConnecting(sock, *it, log) )
          return sock;
    }
    catch(const std::exception &ex)
    {
      log<<"fatal error: "<<ex.what()<<endl;
      throw AnuRandom::ExceptionIO( log.str() );
    }

    // no working resolution found
    log<<"unable to find any working resolution for host '"<<host_<<"' - aborting"<<endl;
    throw AnuRandom::ExceptionIO( log.str() );
  }


  bool tryConnecting(tcp::socket&                               sock,
                     const tcp::resolver::iterator::value_type& endpoint,
                     stringstream&                              log)
  {
    try
    {
      log<<"trying to connect to '"<<endpoint.host_name()<<"'..."<<endl;
      sock.connect(endpoint);
      return true;
    }
    catch(const std::exception &ex)
    {
      log<<"error: "<<ex.what()<<endl;
      return false;
    }
    assert(!"this code is never reached");
    return false;
  }

  string                  host_;
  string                  path_;
  boost::asio::io_service io_;
}; // HttpIo

} // unnamed namespace


//
// API's PIMPL implementation
//

class AnuRandom::Impl
{
public:
  Impl(string&& host, string&& path):
    http_{ move(host), move(path) }
  {
  }

  void read(Data &out)
  {
    std::string html=http_.readContent();
    // TODO
  }

private:
  HttpIo http_;
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


void AnuRandom::read(Data &out)
{
  pimpl_->read(out);
}
