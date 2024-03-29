#include <string>
#include <malloc.h>
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

int key_compress[6][8] = {{14, 17, 11, 24, 1, 5, 3, 28},
                          {15, 6, 21, 10, 23, 19, 12, 4},
                          {26, 8, 16, 7, 27, 20, 13, 2},
                          {41, 52, 31, 37, 47, 55, 30, 40},
                          {51, 45, 33, 48, 44, 49, 39, 56},
                          {34, 53, 46, 42, 50, 36, 29, 32}};

int* compress_permute(int n, int n1, int* pk)  {
  //permuting n bit array pk into n1 bits array new_pk, by using the table key compress
  int* round_pk = new int[n1];
  for (int i=0; i<6; i++) {
    for (int j=0; j<8; j++) {
      int pos = (8*i+j);
      round_pk[pos] = pk[key_compress[i][j]-1];
    }
  }
  return round_pk;
}

//Parity drop also takes place in this
int* permute_key(int* pk) {
  int* per_pk = new int[56];
  int k=0;

  for (int k=0; k<56; ) {
    per_pk[k] = pk[drop_table[k/8][k%8]-1];
    k++;
  }
  return per_pk;
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


int* shift_by_one(int* pk, int start, int finish)  {

  int temp = pk[start];
  for(int i=start+1; i<=finish; i++) {
    pk[i-1] = pk[i];
  }
  pk[finish] = temp;

  return pk;
}

int* circular_left_shift (int* pk, int start, int finish, int round) {
  if(round==1 || round == 2 || round==9 || round==16) { //1 bit shift
    return shift_by_one(pk, start, finish);
  }
  else  {   //2 bit shift
    return shift_by_one(shift_by_one(pk, start, finish), start, finish);
  }
}

int* round_key_generator(int round, int* pk)    {   //
    int n = 56;
    int mid = n/2;
    pk = circular_left_shift(pk, 0, mid-1, round);
    pk = circular_left_shift(pk, mid, n-1, round);
    return pk;
}

int main()  {
  string cipherkey;
  cout<<"Enter 64 bits/8 Bytes cipher key ";
  cin>>cipherkey;
  cipherkey = checkKeySize(cipherkey);
  cout<<endl<<cipherkey<<endl;

  int* ck = convert2bit(cipherkey);
  cout<<"prints the key in the bit string format."<<endl;
  for(int i=0; i<64; i++)
    cout<<ck[i];
  cout<<endl;
  int* per_pk = permute_key(ck);
  cout<<"Parity dropped: "<<endl;
  cout<<"\nPermuted:\n";
  for(int i=0; i<56; i++)
    cout<<per_pk[i];
  cout<<endl;
  /*int* per_pk1 = new int[48];
  per_pk1 = compress_permute(56, 48, per_pk);
  for(int i=0; i<48; i++)
    cout<<per_pk1[i];
  cout<<endl;*/

  cout<<"Key in 1st round: \n";
  int* key1 = new int[56];
  key1 = round_key_generator(3, per_pk);
  for(int i=0; i<56; i++)
    cout<<key1[i];
  cout<<endl;
  int* test;
  test=(int*)malloc(10*sizeof(int));
  for(int i=9;i>=0;i--)
    test[9-i]=i;
  //9876543210
  //test = round_key_generator(1, test);
  //test = circular_left_shift(test, 5, 9, 1);
  //for(int i=0; i<10; i++)
    //cout<<test[i];
  return 0;
}
