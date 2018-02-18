#include <string>
#include <bitset>
#include <iostream>
using namespace std;
int tl[17][32];
int tr[17][32];
int tk[16][48];
int tld[17][32];
int trd[17][32];
int tkd[16][48];
//parity drop
int parity_drop[7][8] = {{57, 49, 41, 33, 25, 17, 9, 1},
    {58, 50, 42, 34, 26, 18, 10, 2},
    {59, 51, 43, 35, 27, 19, 11, 3},
    {60, 52, 44, 36, 63, 55, 47, 39},
    {31, 23, 15, 7, 62, 54, 46, 38},
    {30, 22, 14, 6, 61, 53, 45, 37},
    {29, 21, 13, 5, 28, 20, 12, 4}
};
//compression p-box
int com_p_box[6][8] = {{14, 17, 11, 24, 1, 5, 3, 28},
    {15, 6, 21, 10, 23, 19, 12, 4},
    {26, 8, 16, 7, 27, 20, 13, 2},
    {41, 52, 31, 37, 47, 55, 30, 40},
    {51, 45, 33, 48, 44, 49, 39, 56},
    {34, 53, 46, 42, 50, 36, 29, 32}
};
//expansion p-box
int ex_p_box[8][6]= {{32, 1, 2, 3, 4, 5},
    {4, 5, 6, 7, 8 , 9},
    {8, 9, 10, 11, 12, 13},
    {12, 13, 14, 15, 16, 17},
    {16, 17, 18, 19, 20, 21},
    {20, 21, 22, 23, 24, 25},
    {24, 25, 26, 27, 28, 29},
    {28, 29, 30, 31, 32, 1}
};
//eight s boxes
int sbox[8][4][16]= {{{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },

    {   {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },

    {   {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },

    {   {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },

    {   {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
    },

    {   {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
    },

    {   {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
    },

    {   {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
};
//straight p box
int st_p_box[4][8]= {{16, 7, 20, 21, 29, 12, 28, 17},
    {1, 15, 23, 26, 5, 18, 31, 10},
    {2, 8, 24, 14, 32, 27, 3, 9},
    {19, 13, 30, 6, 22, 11, 4, 25}
};
//initial permutation
int ip[8][8]= {{58, 50, 42, 34, 26, 18, 10, 2},
    {60, 52, 44, 36, 28, 20, 12, 4},
    {62, 54, 46, 38, 30, 22, 14, 6},
    {64, 56, 48, 40, 32, 24, 16, 8},
    {57, 49, 41, 33, 25, 17, 9, 1},
    {59, 51, 43, 35, 27, 19, 11, 3},
    {61, 53, 45, 37, 29, 21, 13, 5},
    {63, 55, 47, 39, 31, 23, 15, 7}
};
//final permutation
int fp[8][8]= {{40, 8, 48, 16, 56, 24, 64, 32},
    {39, 7, 47, 15, 55, 23, 63, 31},
    {38, 6, 46, 14, 54, 22, 62, 30},
    {37, 5, 45, 13, 53, 21, 61, 29},
    {36, 4, 44, 12, 52, 20, 60, 28},
    {35, 3, 43, 11, 51, 19, 59, 27},
    {34, 2, 42, 10, 50, 18, 58, 26},
    {33, 1, 41, 9, 49, 17, 57, 25}
};
//Initial permutation before 16 rounds
int* initial_permutation(int* plaintext)
{
    int* pt=new int[64];
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            pt[i*8+j]=plaintext[ip[i][j]-1];
    return pt;
}
//final permutation after 16 rounds
int* final_permutation(int* plaintext)
{
    int* pt=new int[64];
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            pt[i*8+j]=plaintext[fp[i][j]-1];
    return pt;
}
//applying expansion p box
int* expansion_p_box(int* plaintext)
{
    int* pt = new int[48];
    for(int i=0; i<8; i++)
        for(int j=0; j<6; j++)
            pt[i*6+j]=plaintext[ex_p_box[i][j]-1];
    return pt;
}
//xor operation of n bits
int* xor_op(int* a, int* b, int n)
{
    int *p=new int[n];
    for(int i=0; i<n; i++)
    {
        p[i]=a[i] xor b[i];
    }

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
    for(int i=n-1; i>=0; i--)
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
    for(int i=0; i<8; i++)
    {
        int* a=new int[4];
        int row=convert2dec(plaintext[i*6], plaintext[i*6+5]);
        int column=convert2dec(plaintext[i*6+1], plaintext[i*6+2], plaintext[i*6+3], plaintext[i*6+4]);
        a=convert2bin(sbox[i][row][column], 4);
        for(int j=0; j<4; j++)
            p[i*4+j]=a[j];
    }
    return p;
}
//applying straight p box
int* straight_p_box(int * plaintext)
{
    int* pt = new int[32];
    for(int i=0; i<4; i++)
        for(int j=0; j<8; j++)
            pt[i*8+j]=plaintext[st_p_box[i][j]-1];
    return pt;
}
//initial function
int* func(int* pt, int* key)
{
    int *plaintext=new int[48];
    plaintext=expansion_p_box(pt);
    plaintext=xor_op(plaintext, key, 48);
    int * plain=new int[32];
    plain=s_box(plaintext);
    plain=straight_p_box(plain);
    delete[] plaintext;
    return plain;
}
//each round
int* round(int* l, int *r, int* key)
{
    r=func(r, key);
    l=xor_op(l, r, 32);
    return l;
}
//Takes any string and convert it into bits form into an array of size 64.
int* convert2bit(string myString)
{
    int* plaintext = new int[64];
    int j=0;
    for (std::size_t i = 0; i < myString.size(); ++i)
    {
        std::bitset<8> v8=bitset<8>(myString[i]);
        std::string v8_str = v8.to_string();
        char v[8]= {'0'};
        std::copy(v8_str.begin(), v8_str.end(), v);
        for(int k=0; k<8; k++)
        {
            plaintext[j++] = ((int)(v[k]-'0'));
        }
    }
    return plaintext;
}
//Checks whether the key entered by the user is of 64bits or not.
string checkKeySize(string key)
{
    string res="";
    if (key.length() < 8)
    {
        cout<<"NOT SUFFICIENTLY LONG\n";
        return "\0";
    }
    else
    {
        for (int i=0; i<8; i++)
        {
            res += key[i];
        }
        return res;
    }
}
//Parity drop also takes place in this
int* permute_key(int* pk)
{
    int* per_pk = new int[56];
    int k=0;

    for (int k=0; k<56; )
    {
        per_pk[k] = pk[parity_drop[k/8][k%8]-1];
        k++;
    }
    return per_pk;
}
//permuting n bit array pk into n1 bits array new_pk, by using com_p_box
int* compress_permute(int n, int n1, int* pk)
{
    int* round_pk = new int[n1];
    for (int i=0; i<6; i++)
    {
        for (int j=0; j<8; j++)
        {
            int pos = (8*i+j);
            round_pk[pos] = pk[com_p_box[i][j]-1];
        }
    }
    return round_pk;
}
//circular shift left 1 bit
int* shift_by_one(int* pk, int start, int finish)
{

    int temp = pk[start];
    for(int i=start+1; i<=finish; i++)
    {
        pk[i-1] = pk[i];
    }
    pk[finish] = temp;

    return pk;
}
//circular shift right 1 bit
int* shift_by_one_r(int* pk, int start, int finish)
{
    int temp = pk[finish];
    for(int i=finish-1; i>=start; i--)
    {
        pk[i+1] = pk[i];
    }
    pk[start]=temp;
    return pk;
}
//shift left(0) or right(1) depending on round number
int* circular_shift (int* pk, int start, int finish, int round, int m)
{
    if (m==0)
    {
        if(round==1 || round == 2 || round==9 || round==16)   //1 bit shift left
        {
            return shift_by_one(pk, start, finish);
        }
        else      //2 bit shift left
        {
            return shift_by_one(shift_by_one(pk, start, finish), start, finish);
        }
    }

    else
    {
        if(round==1 || round == 8 || round==15 || round==16)   //1 bit shift right
        {
            return shift_by_one_r(pk, start, finish);
        }
        else      //2 bit shift right
        {
            return shift_by_one_r(shift_by_one_r(pk, start, finish), start, finish);
        }
    }
}
//circular shift of 56 bit key in each round
int* round_key_generator(int round, int* pk, int m)        //
{
    int n = 56;
    int mid = n/2;
    pk = circular_shift(pk, 0, mid-1, round, m);
    pk = circular_shift(pk, mid, n-1, round, m);
    return pk;
}
void bin2hex(int* a, int n)
{
    int in=0;
    for (int i=0;i<n;i+=4)
    {
        in=a[i]*8+a[i+1]*4+a[i+2]*2+a[i+3];
        cout<<std::hex<<in;
    }
    cout<<" ";
}
//encryption algo
int * encrypt(int* plaintext, int *key1)
{
    plaintext=initial_permutation(plaintext);
    for(int j=0; j<16; j++)
    {
        key1 = round_key_generator(j+1, key1, 0);//circular shift
        int* key = new int[48];
        key= compress_permute(56, 48, key1);//compress p box on key
        for(int i=0;i<48;i++)
            tk[j][i]=key[i];
        int* l = new int[32];
        int* r = new int[32];
        int* p = new int[64];
        for(int i=0; i<32; i++)
            l[i]=plaintext[i];
        for(int i=0; i<32; i++)
        {
            r[i]=plaintext[i+32];
            p[i]=plaintext[i+32];
        }
        l=round(l, r, key);
        for(int i=0; i<32; i++)
            p[i+32]=l[i];

        for(int i=0; i<64; i++)
            plaintext[i]=p[i];
        /*cout<<"ciphertext of round "<<j<<endl;
        for(int i=0; i<64; i++)
            if(i%4==0 && i!=0)
                cout<<" "<<plaintext[i];
            else
                cout<<plaintext[i];
        cout<<endl;*/
        for(int i=0;i<32;i++)
        {
            tl[j][i]=plaintext[i];
            tr[j][i]=plaintext[i+32];
        }
        //bin2hex(plaintext, 64);
        //cout<<endl;
        delete[] l;
        delete[] r;
        delete[] p;
    }
    int* l = new int[32];
    int* p = new int[64];
    for(int i=0; i<32; i++)
        l[i]=plaintext[i];
    for(int i=0; i<32; i++)
        p[i]=plaintext[i+32];
    for(int i=0; i<32; i++)
        p[i+32]=l[i];
    for(int i=0; i<64; i++)
        plaintext[i]=p[i];
    plaintext=final_permutation(plaintext);
    for(int i=0;i<32;i++)
        {
            tl[16][i]=plaintext[i];
            tr[16][i]=plaintext[i+32];
        }
    return plaintext;
}
//decryption algo
int * decrypt(int* plaintext, int *key1)
{
    plaintext=initial_permutation(plaintext);
    for(int j=0; j<16; j++)
    {

        int* key = new int[48];
        key= compress_permute(56, 48, key1);//compression p box on key
        key1 = round_key_generator(j+1, key1, 1);//circular shift
        for(int i=0;i<48;i++)
            tkd[j][i]=key[i];
        int* l = new int[32];
        int* r = new int[32];
        int* p = new int[64];
        for(int i=0; i<32; i++)
            l[i]=plaintext[i];
        for(int i=0; i<32; i++)
        {
            r[i]=plaintext[i+32];
            p[i]=plaintext[i+32];
        }
        l=round(l, r, key);
        for(int i=0; i<32; i++)
            p[i+32]=l[i];
        for(int i=0; i<64; i++)
            plaintext[i]=p[i];
        /*cout<<"decrypt round "<<j<<endl;
        for(int i=0; i<64; i++)
            if(i%4==0 && i!=0)
                cout<<" "<<plaintext[i];
            else
                cout<<plaintext[i];
        cout<<endl;*/
        for(int i=0;i<32;i++)
        {
            tld[j][i]=plaintext[i];
            trd[j][i]=plaintext[i+32];
        }
        delete[] l;
        delete[] r;
        delete[] p;
    }
    int* l = new int[32];
    int* p = new int[64];
    for(int i=0; i<32; i++)
        l[i]=plaintext[i];
    for(int i=0; i<32; i++)
        p[i]=plaintext[i+32];
    for(int i=0; i<32; i++)
        p[i+32]=l[i];
    for(int i=0; i<64; i++)
        plaintext[i]=p[i];
    plaintext=final_permutation(plaintext);
    for(int i=0;i<32;i++)
        {
            tld[16][i]=plaintext[i];
            trd[16][i]=plaintext[i+32];
        }
    return plaintext;
}

int main()
{
    string cipherkey, plaintext;
    cout<<"Enter 64 bits/8 characters plaintext ";
    cin>>plaintext;
    cout<<"Enter 64 bits/8 characters cipher key ";
    cin>>cipherkey;
    cipherkey = checkKeySize(cipherkey);
    plaintext = checkKeySize(plaintext);
    if(cipherkey=="\0" || plaintext=="\0")
        return 0;
    cout<<cipherkey<<endl;
    cout<<plaintext<<endl;
    int* ck = convert2bit(cipherkey);
    int* pt = convert2bit(plaintext);

    /*cout<<"Key in the bit string format."<<endl;
    for(int i=0; i<64; i++)
        cout<<ck[i];
    cout<<endl;
    cout<<"Plaintext in the bit string format."<<endl;
    for(int i=0; i<64; i++)
        cout<<pt[i];
    cout<<endl;*/


    int* pk = permute_key(ck);//56bit
    int *ct=new int [64];
    ct=encrypt(pt, pk);
    /*cout<<"Encrypted text:"<<endl;
    for(int i=0; i<64; i++)
        if(i%4==0 && i!=0)
            cout<<" "<<ct[i];
        else
            cout<<ct[i];
    cout<<endl;*/
    int *d=new int [64];
    d=decrypt(ct, pk);
    /*cout<<"\nDecrypted text:"<<endl;
    for(int i=0; i<64; i++)
        if(i%4==0 && i!=0)
            cout<<" "<<d[i];
        else
            cout<<d[i];
    cout<<endl;*/
    cout<<"plaintext: ";
    bin2hex(pt, 64);
    cout<<endl;
    for(int i=0;i<16;i++)
    {
        cout<<"round "<<std::dec<<i+1<<" ";
        bin2hex(tl[i], 32);
        bin2hex(tr[i], 32);
        bin2hex(tk[i], 48);
        cout<<" | ";
        bin2hex(tld[i], 32);
        bin2hex(trd[i], 32);
        bin2hex(tkd[i], 48);
        cout<<endl;
    }
    cout<<"Cipher Text: ";
    bin2hex(tl[16], 32);
    bin2hex(tr[16], 32);
    cout<<"\tDecrypted Text: ";
    bin2hex(tld[16], 32);
    bin2hex(trd[16], 32);
    return 0;
}
