#include <string>
#include <bitset>
#include <iostream>
using namespace std;

int drop_table[7][8] = {{57, 49, 41, 33, 25, 17, 9, 1},
                      {58, 50, 42, 34, 26, 18, 10, 2},
                      {59, 51, 43, 35, 27, 19, 11, 3},
                      {60, 52, 44, 36, 63, 55, 47, 39},
                      {31, 23, 15, 7, 62, 54, 46, 38},
                      {30, 22, 14, 6, 61, 53, 45, 37},
                      {29, 21, 13, 5, 28, 20, 12, 4}};

int* permute_key(int* pk) {
  int* per_pk = new int[64];
  per_pk=pk;
  for(int i=0; i<=6; i++) {
    for (int j=0; j<=7; j++)  {
      per_pk[((i+1)*(j+1)) - 1] = pk[drop_table[i][j]-1];
    }
  }
  
  return per_pk;
}

int* depermute_key(int* per_pk) {
  int* pk1 = new int[56];
  for(int i=0; i<=6; i++) {
    for (int j=0; j<=7; j++)  {
      pk1[drop_table[i][j]-1] = per_pk[((i+1)*(j+1))-1];
    }
  }
  for(int i=0; i<56;i++)
    cout<<pk1[i];
  cout<<endl;;
  return pk1;
}

int* convert2bit(string myString)     
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

//Checks whether the key entered by the user is of 64bits or not.
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

//converts the 64 bits to 56 bits by dropping the parity bits and returning the result into the array pk.
int* paritydrop(int* cipherkey)  {    
  int* pk = new int[56];
  int c = 0;
  for (int i=0; i<64; i++)  {
    if(i==0 || i%8!=0)  {
      pk[c] = cipherkey[i];
      c++;
    }
  }
  return pk;
}

int main()  {
  string cipherkey, plaintext;
  //cout<<"Enter 64 bits/8 Bytes plaintext ";
  //cin>>plaintext;
  cout<<"Enter 64 bits/8 Bytes cipher key ";
  cin>>cipherkey;
  cipherkey = checkKeySize(cipherkey);
  //plaintext = checkKeySize(plaintext);
  cout<<endl<<cipherkey<<endl;
  //cout<<endl<<plaintext<<endl;

  int* ck = convert2bit(cipherkey);
  //int* pt = convert2bit(plaintext);

  cout<<"prints the key in the bit string format."<<endl;
  for(int i=0; i<64;i++)
    cout<<ck[i];
  cout<<endl;

  /*cout<<"prints the plaintext in the bit string format."<<endl;
  for(int i=0; i<64;i++)
    cout<<pt[i];
  cout<<endl;*/

  cout<<"Permuted key: "<<endl;
  int* permuted_pk = permute_key(ck);
  for(int i=0; i<64;i++)
    cout<<permuted_pk[i];
  cout<<endl;;

  cout<<"Parity dropped: "<<endl;
  int* pk = new int[56];
  pk = paritydrop(permuted_pk);
  for(int i=0; i<56;i++)
    cout<<pk[i];
  cout<<endl;
  
  //pk is the final 56 bits private key.
  
  cout<<"Depermuted: "<<endl;
  pk = depermute_key(pk);
  for(int i=0; i<56;i++)
    cout<<pk[i];
  cout<<endl;;
  return 0;
}
