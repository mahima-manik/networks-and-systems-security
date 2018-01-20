#include <string>
#include <bitset>
#include <iostream>
using namespace std;
//expansion p-box
int ex_p_box[8][6]={{32, 1, 2, 3, 4, 5},
                    {4, 5, 6, 7, 8 , 9},
                    {8, 9, 10, 11, 12, 13},
                    {12, 13, 14, 15, 16, 17},
                    {16, 17, 18, 19, 20, 21},
                    {20, 21, 22, 23, 24, 25},
                    {24, 25, 26, 27, 28, 29},
                    {28, 29, 30, 31, 32, 1}};
//eight s boxes
int sbox[8][4][16]={{{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
                  {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
                  {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
                  {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},

                  {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
                  {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
                  {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
                  {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},

                  {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
                  {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
                  {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
                  {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},

                  {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
                  {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
                  {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
                  {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},

                  {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
                  {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
                  {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
                  {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},

                  {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
                  {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
                  {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
                  {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},

                  {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
                  {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
                  {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
                  {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},

                  {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
                  {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
                  {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
                  {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};
//straight p box
int st_p_box[4][8]={{16, 7, 20, 21, 29, 12, 28, 17},
                    {1, 15, 23, 26, 5, 18, 31, 10},
                    {2, 8, 24, 14, 32, 27, 3, 9},
                    {19, 13, 30, 6, 22, 11, 4, 25}};
//applying expansion p box
int* expansion_p_box(int* plaintext)
{
    int* pt = new int[48];
    for(int i=0;i<8;i++)
        for(int j=0;j<6;j++)
            pt[i*6+j]=plaintext[ex_p_box[i][j]-1];
    return pt;
}
//xor operation of n bits
int* xor_op(int* a, int* b, int n)
{
    int *p=new int[n];
    for(int i=0;i<n;i++)
      p[i]=a[i] xor b[i];
    return p;
}
//convert 2 bit binary to decimal
int convert2dec(int a, int b)
{
    int r=a*2+b;
    return r;
}
//convert 4 bit binary to decimal
int convert2dec(int a, int b, int c, int d)
{
    int r=a*8+b*4+2*c+d;
    return r;
}
//convert decimal to n bit binary
int* convert2bin(int a, int n)
{
    int* p=new int[n];
    for(int i=n-1;i>=0;i--)
    {
        p[i]=a%2;
        a=a/2;
    }
    return p;
}
//apply s box
int* s_box(int *plaintext)
{
    int * p=new int[32];
    for(int i=0;i<8;i++)
    {
        int* a=new int[4];
        int row=convert2dec(plaintext[i*6], plaintext[i*6+5]);
        int column=convert2dec(plaintext[i*6+1], plaintext[i*6+2], plaintext[i*6+3], plaintext[i*6+4]);
        a=convert2bin(sbox[i][row][column], 4);
        for(int j=0;j<4;j++)
            p[i*4+j]=a[j];
    }
    return p;
}
//applying straight p box
int* straight_p_box(int * plaintext)
{
    int* pt = new int[32];
    for(int i=0;i<4;i++)
        for(int j=0;j<8;j++)
            pt[i*8+j]=plaintext[st_p_box[i][j]-1];
    return pt;
}
//initial function
int* func(int* pt, int* key)
{
    int *plaintext=new int[48];
    plaintext=expansion_p_box(pt);
    cout<<"Expansion p box applied"<<endl;
    for(int i=0;i<48;i++)
        cout<<plaintext[i];
    plaintext=xor_op(plaintext, key, 48);
    cout<<"xor applied"<<endl;
    for(int i=0;i<48;i++)
        cout<<plaintext[i];
    int * plain=new int[32];
    plain=s_box(plaintext);
    cout<<"s box"<<endl;
    for(int i=0;i<32;i++)
        cout<<plain[i];
    plain=straight_p_box(plain);
    cout<<"straight p box"<<endl;
    for(int i=0;i<32;i++)
        cout<<plain[i];
    delete[] plaintext;
    return plain;
}
//each round
int* round(int* l, int *r, int* key)
{
    r=func(r, key);
    cout<<"func applied"<<endl;
    l=xor_op(l, r, 32);
    cout<<"xor applied"<<endl;
        for(int i=0;i<32;i++)
            cout<<l[i];
        cout<<endl;
    return l;
}

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

int* paritydrop(int* cipherkey)  {    //converts the 64 bits to 56 bits by dropping the parity bits and returning the result into the array pk.
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
int * encrypt(int* plaintext, int *key)
{
//    plaintext=initial_permutation(plaintext);
    for(int j=0;j<16;j++)
    {
        int* l = new int[32];
        int* r = new int[32];
        int* p = new int[64];
        for(int i=0;i<32;i++)
            l[i]=plaintext[i];
        for(int i=0;i<32;i++)
        {
            r[i]=plaintext[i+32];
            p[i]=plaintext[i+32];
        }
        cout<<endl;
        l=round(l, r, key);
        for(int i=0;i<32;i++)
            p[i+32]=l[i];
        plaintext=p;
        cout<<"ciphertext of round "<<j<<endl;
        for(int i=0;i<64;i++)
            cout<<plaintext[i];
        cout<<endl;
        delete[] l;
        delete[] r;
        delete[] p;
    }
//    plaintext=final_permutation(plaintext);
    return plaintext;
}
int * decrypt(int* plaintext, int *key)
{
//    plaintext=initial_permutation(plaintext);
    for(int j=0;j<16;j++)
    {
        int* l = new int[32];
        int* r = new int[32];
        int* p = new int[64];
        for(int i=0;i<32;i++)
        {
            l[i]=plaintext[i];
            p[i+32]=plaintext[i];
        }
        for(int i=0;i<32;i++)
            r[i]=plaintext[i+32];
        r=round(r, l, key);
        for(int i=0;i<32;i++)
            p[i]=r[i];
        plaintext=p;
        delete[] l;
        delete[] r;
        delete[] p;
    }
//    plaintext=final_permutation(plaintext);
    return plaintext;

}
int main()  {
  string cipherkey, plaintext;
  cout<<"Enter 64 bits/8 Bytes plaintext ";
  cin>>plaintext;
  cout<<"Enter 64 bits/8 Bytes cipher key ";
  cin>>cipherkey;
  cipherkey = checkKeySize(cipherkey);
  plaintext = checkKeySize(plaintext);
  cout<<endl<<cipherkey<<endl;
  cout<<endl<<plaintext<<endl;

  int* ck = convert2bit(cipherkey);
  int* pt = convert2bit(plaintext);

  cout<<"prints the key in the bit string format."<<endl;
  for(int i=0; i<64;i++)
    cout<<ck[i];
  cout<<endl;

  cout<<"prints the plaintext in the bit string format."<<endl;
  for(int i=0; i<64;i++)
    cout<<pt[i];
  cout<<endl;

  int* pk = new int[56];
  pk = paritydrop(ck);
  for(int i=0; i<56;i++)
    cout<<pk[i];
  cout<<endl;;
  //pk is the final 56 bits private key.

//temporary
  int *k = new int[48];
  for(int i=0; i<48; i++)
    k[i]=pk[i];
  cout<<"48 bit key: \n";
  for(int i=0; i<48;i++)
    cout<<k[i];
  cout<<endl;;

  int *ct=new int [64];
  ct=encrypt(pt, k);
  cout<<"Ciphertext after 1 round:"<<endl;
  for(int i=0; i<64;i++)
    cout<<ct[i];
  cout<<endl;
  int *d=new int [64];
  d=decrypt(ct, pk);
  cout<<"decrypt after 1 round:"<<endl;
  for(int i=0; i<64;i++)
    cout<<d[i];
  cout<<endl;

  return 0;
}
