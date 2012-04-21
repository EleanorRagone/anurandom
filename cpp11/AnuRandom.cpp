/*
 * implemented by Bartek 'BaSz' Szurgot (http://www.baszerr.eu)
 *
 * note: parts of network IO have been taken from boost::asio examples.
 *
 */
#include <fstream>
#include <sstream>
#include <cassert>
#include <Poco/StreamCopier.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Util/XMLConfiguration.h>

#include "AnuRandom.hpp"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::Util;


//
// helper classes
//
namespace
{

void readHtml(const string &host, const string &path, stringstream &ss)
{
  HTTPClientSession s{host};
  HTTPRequest       request{HTTPRequest::HTTP_GET, path};
  s.sendRequest(request);

  HTTPResponse response;
  istream&     is = s.receiveResponse(response);
  Poco::StreamCopier::copyStream(is, ss);
} // readHtml()


struct HtmlReader: public XMLConfiguration
{
  explicit HtmlReader(istream &is):
    XMLConfiguration{is}
  {
  }
};


void parseFromHtml(stringstream &html, AnuRandom::Data &out)
{
  HtmlReader   xml{html};
  const string str=xml.getString("body.div.table.tr.td");
  copy( str.begin(), str.end(), back_insert_iterator<AnuRandom::Data>{out} );
} // parseFromHtml()

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
    stringstream html;
    readHtml(host_, path_, html);
{                                                               
string tmp=html.str();                                                  
copy( tmp.begin(), tmp.end(), back_insert_iterator<Data>{out} );            
}                                                                       
    //parseHtml( move(html), out );
    parseFromHtml(html, out);
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
