#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>


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



// method declaration :

// 1-- main methods

vector<int> padding(vector<int> input);

vector<Block> parsing(vector<int> input);

vector<Block> expansion(Block block);

vector<string> sha_256(vector<int> input);

vector<Block> preprocess(string txt);

string Bin2Hex(const string &s);


// 2 -- helping methods

vector<int> strToBinary(string txt);

string GetBinaryStringFromHexString(string sHex);


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

vector<int> bitsetToVecInt(bitset<32> b);

vector<int> bitsetToVecInt(bitset<8> b);


static int numberOfBlocks;

int main() {


    sha_256(strToBinary("abc"));


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

    if (c_in == 1)
        result.data.push_back(1);
    std::reverse(result.data.begin(), result.data.end());

    return result;
}

vector<int> add(vector<int> block1, vector<int> block2) {

    vector<int> result;

    int c_in = 0;
    int s = 0;
    int size = max(block1.size(), block2.size());
    for (int i = size - 1; i >= 0; i--) {


        if (size <= block1.size() && size <= block2.size()) {
            s = block1[i] ^ block2[i] ^ c_in;
            c_in = (block1[i] & block2[i]) | (c_in & (block1[i] ^ block2[i]));
        } else if (size > block1.size()) {

            s = c_in ^ block2[i];
            c_in = c_in & block2[i];

        } else {
            s = c_in ^ block1[i];
            c_in = c_in & block1[i];

        }
        result.push_back(s);

    }

    if (c_in == 1)
        result.push_back(1);
    std::reverse(result.begin(), result.end());
    return result;
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
    vector<string> hashValues, k;
    hashValues.emplace_back("6a09e668");
    hashValues.emplace_back("bb67ae85");
    hashValues.emplace_back("3c6ef372");
    hashValues.emplace_back("a54ff53a");
    hashValues.emplace_back("510e527f");
    hashValues.emplace_back("9b05688c");
    hashValues.emplace_back("1f83d9ab");
    hashValues.emplace_back("5be0cd19");

    vector<int> a, b, c, d, e, f, g, h, t1, t2;
    vector<int> pre_a, pre_b, pre_c, pre_d, pre_e, pre_f, pre_g, pre_h;
//        init variables
    a = bitsetToVecInt(bitset<32>(GetBinaryStringFromHexString(hashValues[0])));
    b = bitsetToVecInt(bitset<32>(GetBinaryStringFromHexString(hashValues[1])));
    c = bitsetToVecInt(bitset<32>(GetBinaryStringFromHexString(hashValues[2])));
    d = bitsetToVecInt(bitset<32>(GetBinaryStringFromHexString(hashValues[3])));
    e = bitsetToVecInt(bitset<32>(GetBinaryStringFromHexString(hashValues[4])));
    f = bitsetToVecInt(bitset<32>(GetBinaryStringFromHexString(hashValues[5])));
    g = bitsetToVecInt(bitset<32>(GetBinaryStringFromHexString(hashValues[6])));
    h = bitsetToVecInt(bitset<32>(GetBinaryStringFromHexString(hashValues[7])));

//    todo : initialize vector k

    k.emplace_back("428a2f98");
    k.emplace_back("71374491");
    k.emplace_back("b5c0fbcf");
    k.emplace_back("e9b5dba5");
    k.emplace_back("3956c25b");
    k.emplace_back("59f111f1");
    k.emplace_back("923f82a4");
    k.emplace_back("ab1c5ed5");

    k.emplace_back("d807aa98");
    k.emplace_back("12835b01");
    k.emplace_back("243185be");
    k.emplace_back("550c7dc3");
    k.emplace_back("72be5d74");
    k.emplace_back("80deb1fe");
    k.emplace_back("9bdc06a7");
    k.emplace_back("c19bf174");

    k.emplace_back("e49b69c1");
    k.emplace_back("efbe4786");
    k.emplace_back("0fc19dc6");
    k.emplace_back("240ca1cc");
    k.emplace_back("2de92c6f");
    k.emplace_back("4a7484aa");
    k.emplace_back("5cb0a9dc");
    k.emplace_back("76f988da");

    k.emplace_back("983e5152");
    k.emplace_back("a831c66d");
    k.emplace_back("b00327c8");
    k.emplace_back("bf597fc7");
    k.emplace_back("c6e00bf3");
    k.emplace_back("d5a79147");
    k.emplace_back("06ca6341");
    k.emplace_back("14292967");

    k.emplace_back("27v70a85");
    k.emplace_back("2e1b2138");
    k.emplace_back("4d2c6dfc");
    k.emplace_back("53380d13");
    k.emplace_back("650a7354");
    k.emplace_back("766a0abb");
    k.emplace_back("81c2c92e");
    k.emplace_back("92722c85");

    k.emplace_back("a2bfe8a1");
    k.emplace_back("a81a664b");
    k.emplace_back("c24b8b70");
    k.emplace_back("c76c51a3");
    k.emplace_back("d192e819");
    k.emplace_back("d6990624");
    k.emplace_back("f40e3585");
    k.emplace_back("106aa070");

    k.emplace_back("19a4c116");
    k.emplace_back("1e376c08");
    k.emplace_back("2748774c");
    k.emplace_back("34b0bcb5");
    k.emplace_back("391c0cb3");
    k.emplace_back("4ed8aa4a");
    k.emplace_back("5b9cca4f");
    k.emplace_back("682e6ff3");

    k.emplace_back("748f82ee");
    k.emplace_back("78a563f");
    k.emplace_back("84c87814");
    k.emplace_back("8cc70208");
    k.emplace_back("90befffa");
    k.emplace_back("a4506ceb");
    k.emplace_back("bef9a3f7");
    k.emplace_back("c67178f2");


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


        for (int j = 0; j < 64; j++) {

            vector<int> chv = ch(e, f, g);
            vector<int> majv = maj(a, b, c);
            bigSimga0(a);
            vector<int> k_int = bitsetToVecInt(bitset<32>(GetBinaryStringFromHexString(k[j])));
            t1 = add(add(add(h, bigSimga1(e)), add(ch(e, f, g), k_int)), w[j].data);
            t2 = add(bigSimga0(a), maj(a, b, c));
            h = g;
            g = f;
            f = e;
            e = add(d, t1);
            d = c;
            c = b;
            b = a;
            a = add(t1, t2);
        }

//         updating has values


        a = add(a, pre_a);
        b = add(b, pre_b);
        c = add(c, pre_c);
        d = add(d, pre_d);
        e = add(e, pre_e);
        f = add(f, pre_f);
        g = add(g, pre_g);
        h = add(h, pre_h);
        for (int l = 0; l < a.size(); ++l) {
            cout << a[l];

        }

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

    for (int i = 0; i < min(x.size(), y.size()); i++) {

        result.push_back(x[i] & y[i]);

    }

    return result;


}

vector<int> notGate(vector<int> x) {
    vector<int> result;

    for (int i = 0; i < x.size(); i++) {

        result.push_back(not x[i]);

    }

    return result;

}

vector<int> bitsetToVecInt(bitset<32> b) {

    vector<int> res;
    for (int i = 0; i < b.size(); i++) {
        res.push_back((int) b[i]);

    }
    return res;

}

vector<int> bitsetToVecInt(bitset<8> b) {

    vector<int> res;
    for (int i = 0; i < b.size(); i++) {
        res.push_back((int) b[i]);

    }
    return res;

}

vector<int> strToBinary(string txt) {

    vector<int> result;
    for (int i = 0; i < txt.size(); i++) {

        vector<int> tmp = bitsetToVecInt(bitset<8>(txt[i]));
        for (int j = 0; j < tmp.size(); ++j) {
            result.push_back(tmp[j]);

        }
    }

    return result;

}


