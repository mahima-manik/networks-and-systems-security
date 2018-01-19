#include <string>
#include <bitset>
#include <iostream>
using namespace std;


string convert2bit(string myString)
{
    string plaintext;
    int j=0;
    for (std::size_t i = 0; i < myString.size(); ++i)
    {
      std::bitset<8> v8=bitset<8>(myString[i]);
      std::string v8_str = v8.to_string();
      char v[8]={'0'};
      std::copy(v8_str.begin(), v8_str.end(), v);
      for(int k=0;k<8;k++){
        plaintext.push_back((char)(v[k]-'0'));
        //cout<<((int)(v[k]-'0'));
      }
    }
    cout<<plaintext<<endl;
    return plaintext;
}

string checkKeySize(string key) {
  string res="";
  if (key.length() < 8) {
    return "NOT SUFFICIENTLY LONG\n";
  }
  else  {
    for (int i=0; i<8; i++)  {
      res += key[i];
    }
    return res;
  }
}



int main()  {
  string cipherkey;
  cout<<"Enter 64 bits/8 Bytes cipher key ";
  cin>>cipherkey;
  cipherkey = checkKeySize(cipherkey);
  cout<<endl<<cipherkey<<endl;
  string plaintext = convert2bit(cipherkey);
  cout<<plaintext;
  return 0;
}