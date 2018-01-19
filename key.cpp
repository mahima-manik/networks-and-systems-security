#include <string>
#include <bitset>
#include <iostream>
using namespace std;


int* convert2bit(string myString)     //Takes any string and convert it into bits form into an array of size 64.
{
    int* plaintext = new int[64];
    int j=0;
    for (std::size_t i = 0; i < myString.size(); ++i)
    {
      std::bitset<8> v8=bitset<8>(myString[i]);
      std::string v8_str = v8.to_string();
      char v[8]={'0'};
      std::copy(v8_str.begin(), v8_str.end(), v);
      for(int k=0;k<8;k++){
        plaintext[j++] = ((int)(v[k]-'0'));
      }
    }
    return plaintext;
}

string checkKeySize(string key) {     //Checks whether the key entered by the user is of 64bits or not. 
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

int* paritydrop(int* plaintext)  {    //converts the 64 bits to 56 bits by dropping the parity bits and returning the result into the array pk.
  int* pk = new int[56];
  int c = 0;
  for (int i=0; i<64; i++)  {
    if(i==0 || i%8!=0)  {  
      pk[c] = plaintext[i];
      c++;
    }
  }
  return pk;
}

int main()  {
  string cipherkey;
  cout<<"Enter 64 bits/8 Bytes cipher key ";
  cin>>cipherkey;
  cipherkey = checkKeySize(cipherkey);
  cout<<endl<<cipherkey<<endl;
  
  int* plaintext = convert2bit(cipherkey);
  //prints the key in the bit string format.
  for(int i=0; i<64;i++)  
    cout<<plaintext[i];
  cout<<endl;
  
  int* pk = new int[56];
  pk = paritydrop(plaintext);
  for(int i=0; i<56;i++)
    cout<<pk[i];
  //pk is the final 56 bits private key.
  return 0;
}
