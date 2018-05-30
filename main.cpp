#include <iostream>
#include <vector>
#include <bitset>
#include <iomanip>
#include <sstream>
#include <bitset>
#include <string>


using namespace std;

class Block_Header {
    int version[32];
    int hashPrevBlock[512];
    int hashMerkelRoot[512];
    int time[32];
    int difficulty[32];
    int nonce[32];

};

class Block {
public :
    vector<int> data;
//    Block_Header header;

};


vector<int> padding(vector<int> input);

vector<Block> parsing(vector<int> input);

vector<Block> expansion(Block block);

vector<int> strToBinary(string txt);

string GetBinaryStringFromHexString(string sHex);

vector<string> sha_256();

vector<Block> preprocess(string txt);

Block ROT(Block block, int n);

vector<int> ROT(vector<int> block, int n);

Block SHF(Block block, int n);

vector<int> SHF(vector<int> block, int n);

Block sigma0(Block block);

Block sigma1(Block block);

vector<int> bigSimga0(vector<int> x);

vector<int> bigSimga1(vector<int> x);

vector<int> bigSimga2(vector<int> x);

Block XOR(Block block1, Block block2);

vector<int> XOR(vector<int> block1, vector<int> block2);

Block add(Block block1, Block block2);
vector<int> add(vector<int> block1, vector<int> block2);


vector<int> ch(vector<int> x, vector<int> y, vector<int> z);

vector<int> maj(vector<int> x, vector<int> y, vector<int> z);

vector<int> andGate(vector<int> x, vector<int> y);

vector<int> notGate(vector<int> x);
vector<int> bitsetToVecInt ( bitset<32> b);


static int numberOfBlocks;

int main() {
    vector<int> x;
    x.push_back(1);
    x.push_back(0);
    x.push_back(1);
    x.push_back(1);


    vector<int> v = padding(x);
    int sum = 0;
    for (int i = 0; i < v.size(); ++i) {
        if (v[i] == 1) {
            sum++;
            int a = v[i];
//           cout<< a  <<endl;


        }

//        std::cout <<v[i]<< endl;
    }

//    std::cout <<sum<< endl;
//    string str  ="1010";

//    std::cout << std :: hex << ((unsigned char) 12 + 4)   << endl;


    string s = "6a09e668";
    string s1 = GetBinaryStringFromHexString(s);
    bitset<32> s2(s1);
    cout << s2 << endl;
    vector<int> a ,b;
    a.push_back(1);
    a.push_back(0);
    a.push_back(1);
    a.push_back(0);
    a.push_back(1);
    a.push_back(0);
    a.push_back(1);

    b.push_back(1);
    b.push_back(0);
    b.push_back(1);
    b.push_back(1);
    b.push_back(1);
    b.push_back(0);
    b.push_back(0);


    a = andGate( a, b );

    for (int j = 0; j < a.size(); j++) {
        cout <<   a[j];
    }

    return 0;
}

vector<int> padding(vector<int> input) {


    vector<int> tmpInput = input;

    tmpInput.push_back(1);

    while (tmpInput.size() % 512 != 448) {
        tmpInput.push_back(0);
    }

    std::string binary = std::bitset<64>(input.size()).to_string(); //to binary

    for (int i = 0; i < binary.size(); i++) {
        tmpInput.push_back((binary[i]));
//        std::cout <<binary[i]<< endl;
    }


    return tmpInput;
}


vector<Block> parsing(vector<int> input) {

    numberOfBlocks = static_cast<int>(input.size() / 512);
    vector<Block> blocks;
    for (int i = 1; i <= numberOfBlocks; i++) {
        Block block;
        for (int j = (i - 1) * 512; j < i * 512; j++) {
            block.data.push_back(input[j]);

        }

        blocks.push_back(block);

    }

    return blocks;
}

Block ROT(Block block, int n) {

    vector<int> tempData = block.data;
    vector<int> data = block.data;

    int j = 1;
    while (j <= n) {
        for (int i = 0; i < data.size(); i++) {

            if (i == tempData.size() - 1) {

                tempData[0] = data[i];

            } else {

                tempData[i + 1] = data[i];
            }

        }

        data = tempData;
        j++;

    }

    block.data = data;
    return block;

}

vector<int> ROT(vector<int> block, int n) {

    vector<int> tempData = block;
    vector<int> data = block;

    int j = 1;
    while (j <= n) {
        for (int i = 0; i < data.size(); i++) {

            if (i == tempData.size() - 1) {

                tempData[0] = data[i];

            } else {

                tempData[i + 1] = data[i];
            }

        }

        data = tempData;
        j++;

    }

    block = data;
    return block;

}


Block SHF(Block block, int n) {

    int last = static_cast<int>(block.data.size() - 1);

    for (int i = last - n; i >= 0; i--) {


        block.data[i + n] = block.data[i];


    }

    for (int j = 0; j < n; ++j) {

        block.data[j] = 0;

    }


    return block;


}

vector<int> SHF(vector<int> block, int n) {

    int last = static_cast<int>(block.size() - 1);

    for (int i = last - n; i >= 0; i--) {


        block[i + n] = block[i];


    }

    for (int j = 0; j < n; ++j) {

        block[j] = 0;

    }


    return block;


}

vector<Block> expansion(Block block) {

    vector<Block> blocks;

    for (int i = 0; i < 16; i++) {

        Block b;

        for (int j = i * 32; j < (i + 1) * 32; j++) {

            b.data.push_back(block.data[j]);

        }
        std::reverse(b.data.begin(), b.data.end());
        blocks.push_back(b);

    }


    for (int i = 16; i < 64; i++) {


        Block b;

        Block temp1 = add(sigma1(blocks[i - 2]), blocks[i - 7]);
        Block temp2 = add(sigma0(blocks[i - 15]), blocks[i - 16]);

        b = add(temp1, temp2);
        std::reverse(b.data.begin(), b.data.end());
        blocks.push_back(b);
    }


    return blocks;
}

Block XOR(Block block1, Block block2) {

    Block result;

    for (int i = 0; i < block1.data.size(); i++) {

        result.data.push_back(block1.data[i] ^ block2.data[i]);

    }

    return result;

}


vector<int> XOR(vector<int> block1, vector<int> block2) {

    vector<int> result;

    for (int i = 0; i < block1.size(); i++) {

        result.push_back(block1[i] ^ block2[i]);

    }

    return result;

}

Block add(Block block1, Block block2) {

    Block result;

    int c_in = 0;
    int s = 0;
    for (int i = block1.data.size() - 1; i >= 0; i--) {

        s = block1.data[i] ^ block2.data[i] ^ c_in;
        c_in = (block1.data[i] & block2.data[i]) | (c_in & (block1.data[i] ^ block2.data[i]));

        result.data.push_back(s);

    }

    if ( c_in == 1)
        result.data.push_back(1);
    std:: reverse(result.data.begin() , result.data.end());

    return result;
}

vector<int> add(vector<int> block1, vector<int> block2) {

    vector<int> result;

    int c_in = 0;
    int s = 0;
    for (int i = block1.size() - 1; i >=  0 ; i--) {

        s = block1[i] ^ block2[i] ^ c_in;
        c_in = (block1[i] & block2[i]) | (c_in & (block1[i] ^ block2[i]));

        result.push_back(s);

    }

    if ( c_in == 1)
        result.push_back(1);
    std:: reverse(result.begin() , result.end());
    return  result;
}

Block sigma0(Block block) {

    Block result;

    Block temp = XOR(ROT(block, 17), ROT(block, 14));
    result = XOR(temp, SHF(block, 12));
    return result;


}

Block sigma1(Block block) {


    Block result;

    Block temp = XOR(ROT(block, 19), ROT(block, 9));

    result = XOR(temp, SHF(block, 9));
    return result;
}

string GetBinaryStringFromHexString(string sHex) {
    string sReturn = "";
    for (int i = 0; i < sHex.length(); ++i) {
        switch (sHex[i]) {
            case '0':
                sReturn.append("0000");
                break;
            case '1':
                sReturn.append("0001");
                break;
            case '2':
                sReturn.append("0010");
                break;
            case '3':
                sReturn.append("0011");
                break;
            case '4':
                sReturn.append("0100");
                break;
            case '5':
                sReturn.append("0101");
                break;
            case '6':
                sReturn.append("0110");
                break;
            case '7':
                sReturn.append("0111");
                break;
            case '8':
                sReturn.append("1000");
                break;
            case '9':
                sReturn.append("1001");
                break;
            case 'a':
                sReturn.append("1010");
                break;
            case 'b':
                sReturn.append("1011");
                break;
            case 'c':
                sReturn.append("1100");
                break;
            case 'd':
                sReturn.append("1101");
                break;
            case 'e':
                sReturn.append("1110");
                break;
            case 'f':
                sReturn.append("1111");
                break;
        }
    }
    return sReturn;
}

vector<string> sha_256(vector<int> input) {

    vector<int> paddingResult = padding(input);
    vector<Block> blocks = parsing(paddingResult);
    auto N = static_cast<int>(blocks.size());
    vector<string> hashValues;
    hashValues.emplace_back("6a09e668");
    hashValues.emplace_back("bb67ae85");
    hashValues.emplace_back("3c6ef372");
    hashValues.emplace_back("a54ff53a");
    hashValues.emplace_back("510e527f");
    hashValues.emplace_back("9b05688c");
    hashValues.emplace_back("1f83d9ab");
    hashValues.emplace_back("5be0cd19");

    vector<int> a, b, c, d, e, f, g, h , t1 , t2 ;
    vector<int> pre_a, pre_b, pre_c, pre_d, pre_e, pre_f, pre_g, pre_h;
//        init variables
    a = bitsetToVecInt(bitset<32>(GetBinaryStringFromHexString(hashValues[0])) );
    b = bitsetToVecInt(bitset<32>(GetBinaryStringFromHexString(hashValues[1])) );
    c = bitsetToVecInt(bitset<32>(GetBinaryStringFromHexString(hashValues[2])) );
    d = bitsetToVecInt(bitset<32>(GetBinaryStringFromHexString(hashValues[3])) );
    e = bitsetToVecInt(bitset<32>(GetBinaryStringFromHexString(hashValues[4])) );
    f = bitsetToVecInt(bitset<32>(GetBinaryStringFromHexString(hashValues[5])) );
    g = bitsetToVecInt(bitset<32>(GetBinaryStringFromHexString(hashValues[6])) );
    h = bitsetToVecInt(bitset<32>(GetBinaryStringFromHexString(hashValues[7])) );

//    todo : initialize vector k

    for (int i = 0; i < N; i++) {


        pre_a = a;
        pre_b = b;
        pre_c = c;
        pre_d = d;
        pre_e = e;
        pre_f = f;
        pre_g = g;
        pre_h = h;
        vector<Block> w = expansion(blocks[i]);



        for (int j = 0; j < 64 ; j++) {

            vector<int> chv= ch(e,f,g);
            vector<int> majv = maj( a,b,c);
            bigSimga0(a);

//         todo :   t1 = add(add( add(h,bigSimga1(e))  ,  add(ch(e,f,g), kj)  ), w[j].data  );
            t2 = add(bigSimga0(a) , maj(a,b,c));
            h = g;
            g = f;
            f = e;
            e = add(d, t1);
            d = c;
            c = b;
            b = a;
            a = add( t1, t2);
        }

//         updating has values


        a = add(a,pre_a );
        b = add(b,pre_b );
        c = add(c,pre_c);
        d = add(d,pre_d );
        e = add(e,pre_e );
        f = add(f,pre_f );
        g = add(g,pre_g );
        h = add(h,pre_h );


    }


//    TODO : update hashvalues : convert a-b-c-d-e-f-g-h to hex String and put them in the hashvalues vector

     return hashValues;


}

vector<int> bigSimga0(vector<int> x) {

    vector<int> result, temp1, temp2;
    temp1 = XOR(ROT(x, 2), ROT(x, 13));
    temp2 = XOR(ROT(x, 22), SHF(x, 7));
    result = XOR(temp1, temp2);
    return result;


}


vector<int> bigSimga1(vector<int> x) {

    vector<int> result, temp1;
    temp1 = XOR(ROT(x, 6), ROT(x, 11));
    result = XOR(temp1, ROT(x, 25));
    return result;


}

vector<int> bigSimga2(vector<int> x) {

    vector<int> result, temp1, temp2;
    temp1 = XOR(ROT(x, 2), ROT(x, 3));
    temp2 = XOR(ROT(x, 15), SHF(x, 5));
    result = XOR(temp1, temp2);
    return result;


}

vector<int> ch(vector<int> x, vector<int> y, vector<int> z) {
    vector<int> temp1, res;
    temp1 = XOR(andGate(x, y), andGate(notGate(y), z));
    res = XOR(temp1, andGate(notGate(x), z));
    return res;
}

vector<int> maj(vector<int> x, vector<int> y, vector<int> z) {

    vector<int> temp1, res;
    temp1 = XOR(andGate(x, z), andGate(x, z));

    res = XOR(temp1, andGate(y, z));

    return res;


}

vector<int> andGate(vector<int> x, vector<int> y) {


    vector<int> result;

    for (int i = 0; i < x.size(); i++) {

        result.push_back(x[i] & y[i]);

    }

    return result;


}

vector<int> notGate(vector<int> x) {
    vector<int> result;

    for (int i = 0; i < x.size(); i++) {

        result.push_back( not x[i]);

    }

    return result;

}

vector<int> bitsetToVecInt ( bitset<32> b){

    vector<int> res;
    for (int i = 0; i < b.size(); i++) {
        res.push_back((int)b[i]);

    }
    return res;

}
