#include <iostream>

#include "AnuRandom.hpp"

using namespace std;


int main(void)
{
  //AnuRandom rnd("www.onet.pl");
  AnuRandom rnd("213.180.141.140");
  //AnuRandom rnd("127.0.0.1");
  AnuRandom::Data data=rnd.read();
  cout<<"got "<<data.size()<<" bytes"<<endl;
  return 0;
}
