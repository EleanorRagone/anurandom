/*
 * implemented by Bartek 'BaSz' Szurgot (http://www.baszerr.eu)
 *
 * note: parts of network IO have been taken from boost::asio examples.
 *
 */
#include <sstream>
#include <cassert>
#include <Poco/StreamCopier.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPClientSession.h>

#include "AnuRandom.hpp"

using namespace std;
using namespace Poco::Net;


//
// helper classes
//
namespace
{

string readHtml(const string &host, const string &path)
{
  HTTPClientSession s{host};
  HTTPRequest       request{HTTPRequest::HTTP_GET, path};
  s.sendRequest(request);

  HTTPResponse response;
  istream&     is = s.receiveResponse(response);
  stringstream ss;
  Poco::StreamCopier::copyStream(is, ss);

  return ss.str();
} // readHtml()


void parseFromHtml(const string html, AnuRandom::Data &out)
{
}

} // unnamed namespace


//
// API's PIMPL implementation
//

class AnuRandom::Impl
{
public:
  Impl(string&& host, string&& path):
    host_{ move(host) },
    path_{ fixPath( move(path) ) }
  {
  }

  void read(Data &out)
  {
    string html=readHtml(host_, path_);
    //parseHtml( move(html), out );
    parseFromHtml(html, out);
    copy( html.begin(), html.end(), back_insert_iterator<Data>{out} );
    // TODO
  }

private:
  // ensure path starts with the leading '/'
  string fixPath(string in) const
  {
    if(in.empty() || in[0]!='/')
      return "/"+move(in);
    return move(in);
  }

  const string host_;
  const string path_;
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
