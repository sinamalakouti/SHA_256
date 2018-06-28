#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <chrono>
#include <windows.h>
#include <time.h>
#include <math.h>
#include <psapi.h>

#define version "02000000"
#define prev_block "17975b97c18ed1f7e255adf297599b55330edab87803c8170100000000000000"
#define time_stamp "358b0553"
#define diff "5350f119"

static int hash = 1;
static int nonce = 0;

using namespace std;

class Block {
public :
    vector<int> data;
};


// method declaration :

// 1-- main methods

void init();

double Cpu();

SIZE_T Memory();

vector<int> padding(vector<int> input);

vector<Block> parsing(vector<int> input);

vector<Block> expansion(Block block);

string sha_256(vector<int> input);

// 2 -- helping methods
Block ROT(Block block, int n);

vector<int> ROT(vector<int> block, int n);

Block SHF(Block block, int n);

vector<int> SHF(vector<int> block, int n);

Block sigma0(Block block);

Block sigma1(Block block);

vector<int> bigSimga0(vector<int> x);

vector<int> bigSimga1(vector<int> x);

vector<int> bigSimga2(vector<int> x);

vector<int> ch(vector<int> x, vector<int> y, vector<int> z);

vector<int> maj(vector<int> x, vector<int> y, vector<int> z);

vector<int> andGate(vector<int> x, vector<int> y);

vector<int> notGate(vector<int> x);

vector<int> XOR(vector<int> block1, vector<int> block2);

Block XOR(Block block1, Block block2);

vector<int> bitsetToVecInt(bitset<8> b);

vector<int> bitsetToVecInt(bitset<32> b);

vector<int> bitsetToVecInt(bitset<64> b);

vector<int> bitsetToVecInt(bitset<256> b);

vector<int> bitsetToVecInt(bitset<640> b);

vector<int> multiply(int n, vector<int> x);

vector<int> sub(vector<int> x, vector<int> y);

Block add(Block block1, Block block2);

vector<int> add(vector<int> block1, vector<int> block2);

vector<int> updateNumber(int size, vector<int> number);

string binToHex(vector<int> binArr);

vector<int> strToBinary(string txt);

string hexToBin(string sHex);

string intToChar(int a);

vector<int> stringToIntVector(string text);

bool lessThan(vector<int> b1, vector<int> b2);

vector<int> updateBlockHeader(vector<int> block_header, vector<int> nonce);

static int numberOfBlocks;

static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;

void init(){
    SYSTEM_INFO sysInfo;
    FILETIME ftime, fsys, fuser;
    GetSystemInfo(&sysInfo);
    numProcessors = sysInfo.dwNumberOfProcessors;
    GetSystemTimeAsFileTime(&ftime);
    memcpy(&lastCPU, &ftime, sizeof(FILETIME));
    self = GetCurrentProcess();
    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

double Cpu(){
    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;
    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));
    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));
    percent = (sys.QuadPart - lastSysCPU.QuadPart) +
              (user.QuadPart - lastUserCPU.QuadPart);
    percent /= (now.QuadPart - lastCPU.QuadPart);
    percent /= numProcessors;
    lastCPU = now;
    lastUserCPU = user;
    lastSysCPU = sys;
    return percent * 100;
}

//SIZE_T Memory(){
//    PROCESS_MEMORY_COUNTERS_EX pmc;
//    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
//    return pmc.PrivateUsage;
//}

int main() {

    init();

    clock_t firstSeconds =  clock();

    string merkel_root =  sha_256(strToBinary("abcd"));

    cout << merkel_root + " merkel_root"<< endl;

    string block_header = "";
    block_header.append(version);
    block_header.append(prev_block);
    block_header.append(merkel_root);
    block_header.append(time_stamp);
    block_header.append(diff);
    block_header.append("00000000");

    vector<int> blockHeader = stringToIntVector(hexToBin(block_header));
    vector<int> whileInput;

    vector<int> nonce;
    for (int i = 0; i < 32; i++)
        nonce.push_back(0);

    vector<int> one;
    one.push_back(1);

    one = updateNumber(nonce.size(), one);

    string target = "1010101010101010101010101010101010101010101010101010101010101010";
    //string target = "00000000000002816E0000000000000000000000000000000000000000000000";

    string hashResult = "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";

    while (lessThan(stringToIntVector(hexToBin(hashResult)), stringToIntVector(hexToBin(target))) == false) {
        whileInput = updateBlockHeader(blockHeader,nonce);
        string tempHash = sha_256(whileInput);
        hashResult = sha_256(stringToIntVector(hexToBin(tempHash)));
        nonce = add(nonce, one);
    }

    cout<<hashResult<<endl;

    clock_t secondSeconds =  clock();

//    printf("\n\n%f seconds \n%f MB \n%f cpu percentage\n",
//           ((float)secondSeconds - (float)firstSeconds)/1000.0, Memory()/1000000.0 , Cpu());

    return 0;
}

vector<int> padding(vector<int> input) {

    int size = input.size();

    vector<int> tmpInput = input;

    tmpInput.push_back(1);

    while (tmpInput.size() % 512 != 448) {
        tmpInput.push_back(0);
    }

    vector<int> binary = bitsetToVecInt(bitset<64>(size)); //to binary

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

//        std::reverse(b.data.begin(), b.data.end());
        blocks.push_back(b);

    }


    for (int i = 16; i < 64; i++) {


        Block b;

        Block temp1 = add(sigma1(blocks[i - 1]), blocks[i - 6]);
        Block temp2 = add(sigma0(blocks[i - 12]), blocks[i - 15]);
//standard :
//        Block temp1 = add(sigma1(blocks[i - 2]), blocks[i - 7]);
//        Block temp2 = add(sigma0(blocks[i - 15]), blocks[i - 16]);


        b = add(temp1, temp2);
//        std::reverse(b.data.begin(), b.data.end());
        blocks.push_back(b);
    }


    for (int k = 0; k < blocks.size(); ++k) {

        Block b = blocks[k];
        vector<int> temp;
        for (int k = b.data.size() - 1; k >= 24; k--) {
            temp.push_back(b.data[k]);
        }

        for (int k = 16; k <= 23; k++) {
            temp.push_back(b.data[k]);
        }
        for (int k = 15; k >= 0; k--) {
            temp.push_back(b.data[k]);
        }
        b.data = temp;
        blocks[k] = b;

// standard
//      blocks[k] = b;
    }

    return blocks;
}

Block XOR(Block block1, Block block2) {


    Block result;

    int minSize = min(block1.data.size(), block2.data.size());
    std::reverse(block1.data.begin(), block1.data.end());
    std::reverse(block2.data.begin(), block2.data.end());

    for (int i = 0; i < minSize; i++) {

        result.data.push_back(block1.data[i] ^ block2.data[i]);
    }

    std::reverse(result.data.begin(), result.data.end());

    return result;

}

vector<int> XOR(vector<int> x, vector<int> y) {


    vector<int> result;

    int minSize = min(x.size(), y.size());
    std::reverse(x.begin(), x.end());
    std::reverse(y.begin(), y.end());

    for (int i = 0; i < minSize; i++) {

        result.push_back(x[i] ^ y[i]);
    }

    std::reverse(result.begin(), result.end());
    return result;
}

Block add(Block block1, Block block2) {

    Block result;

    int c_in = 0;
    int s = 0;
    int size = min(block1.data.size(), block2.data.size());
    std::reverse(block1.data.begin(), block1.data.end());
    std::reverse(block2.data.begin(), block2.data.end());
    for (int i = 0; i < size; i++) {

        s = block1.data[i] ^ block2.data[i] ^ c_in;
        c_in = (block1.data[i] & block2.data[i]) | (c_in & (block1.data[i] ^ block2.data[i]));

        result.data.push_back(s);
    }


    std::reverse(result.data.begin(), result.data.end());

    return result;
}

vector<int> add(vector<int> block1, vector<int> block2) {

    vector<int> result;

    int c_in = 0;
    int s = 0;
    int size = min(block1.size(), block2.size());
    std::reverse(block1.begin(), block1.end());
    std::reverse(block2.begin(), block2.end());
    for (int i = 0; i < size; i++) {


//        if (size <= block1.size() && size <= block2.size()) {
        s = block1[i] ^ block2[i] ^ c_in;
        c_in = (block1[i] & block2[i]) | (c_in & (block1[i] ^ block2[i]));
//        } else if (size > block1.size()) {

//            s = c_in ^ block2[i];
//            c_in = c_in & block2[i];

//        } else {
//            s = c_in ^ block1[i];
//            c_in = c_in & block1[i];

//        }


        result.push_back(s);

    }

//    if (c_in == 1)
//        result.push_back(1);
    std::reverse(result.begin(), result.end());
    return result;
}

Block sigma0(Block block) {

    Block result;

    Block temp = XOR(ROT(block, 17), ROT(block, 14));
    result = XOR(temp, SHF(block, 12));

//    standard
//    Block temp = XOR(ROT(block, 7), ROT(block, 18));
//    result = XOR(temp, SHF(block, 3));

    return result;


}

Block sigma1(Block block) {

    Block result,temp;

    temp = XOR(ROT(block, 9), ROT(block, 19));
    result = XOR(temp, SHF(block, 9));

//    standard :
//     temp = XOR(ROT(block, 17), ROT(block, 19));
//     result = XOR(temp, SHF(block, 10));

    return result;
}


string binToHex(vector<int> binArr) {
    string result = "";
    string temp = "";
    for (int i = 1; i <= binArr.size()/4; i++) {
        temp = "";
        for (int j = ((i - 1) * 4); j <= (i * 4) - 1; j++) {
            temp.append(intToChar(binArr[j]));
        }
        if (temp.compare("0000") == 0) {
            result.append("0");
        } else if (temp.compare("0001") == 0) {
            result.append("1");
        } else if (temp.compare("0010") == 0) {
            result.append("2");
        } else if (temp.compare("0011") == 0) {
            result.append("3");
        } else if (temp.compare("0100") == 0) {
            result.append("4");
        } else if (temp.compare("0101") == 0) {
            result.append("5");
        } else if (temp.compare("0110") == 0) {
            result.append("6");
        } else if (temp.compare("0111") == 0) {
            result.append("7");
        } else if (temp.compare("1000") == 0) {
            result.append("8");
        } else if (temp.compare("1001") == 0) {
            result.append("9");
        } else if (temp.compare("1010") == 0) {
            result.append("a");
        } else if (temp.compare("1011") == 0) {
            result.append("b");
        } else if (temp.compare("1100") == 0) {
            result.append("c");
        } else if (temp.compare("1101") == 0) {
            result.append("d");
        } else if (temp.compare("1110") == 0) {
            result.append("e");
        } else if (temp.compare("1111") == 0) {
            result.append("f");
        }
    }

    return result;

}

string intToChar(int a) {

    if (a == 1)
        return "1";
    else return "0";
}

string hexToBin(string sHex) {
    string sReturn = "";
    for (int i = 0; i < sHex.length(); i++) {
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

string sha_256(vector<int> input) {

    vector<int> paddingResult = padding(input);

    vector<Block> blocks = parsing(paddingResult);

    auto N = static_cast<int>(blocks.size());

    vector<int> a, b, c, d, e, f, g, h, t1, t2;
    vector<int> pre_a, pre_b, pre_c, pre_d, pre_e, pre_f, pre_g, pre_h;
//        init variables
    pre_a = bitsetToVecInt(bitset<32>(hexToBin("6a09e667")));
    pre_b = bitsetToVecInt(bitset<32>(hexToBin("bb67ae85")));
    pre_c = bitsetToVecInt(bitset<32>(hexToBin("3c6ef372")));
    pre_d = bitsetToVecInt(bitset<32>(hexToBin("a54ff53a")));
    pre_e = bitsetToVecInt(bitset<32>(hexToBin("510e527f")));
    pre_f = bitsetToVecInt(bitset<32>(hexToBin("9b05688c")));
    pre_g = bitsetToVecInt(bitset<32>(hexToBin("1f83d9ab")));
    pre_h = bitsetToVecInt(bitset<32>(hexToBin("5be0cd19")));

//    todo : initialize vector k

    static const string k[64] = {"428a2f98", "71374491", "b5c0fbcf", "e9b5dba5", "3956c25b", "59f111f1", "923f82a4",
                                 "ab1c5ed5",
                                 "d807aa98", "12835b01", "243185be", "550c7dc3", "72be5d74", "80deb1fe", "9bdc06a7",
                                 "c19bf174",
                                 "e49b69c1", "efbe4786", "0fc19dc6", "240ca1cc", "2de92c6f", "4a7484aa", "5cb0a9dc",
                                 "76f988da",
                                 "983e5152", "a831c66d", "b00327c8", "bf597fc7", "c6e00bf3", "d5a79147", "06ca6351",
                                 "14292967",
                                 "27b70a85", "2e1b2138", "4d2c6dfc", "53380d13", "650a7354", "766a0abb", "81c2c92e",
                                 "92722c85",
                                 "a2bfe8a1", "a81a664b", "c24b8b70", "c76c51a3", "d192e819", "d6990624", "f40e3585",
                                 "106aa070",
                                 "19a4c116", "1e376c08", "2748774c", "34b0bcb5", "391c0cb3", "4ed8aa4a", "5b9cca4f",
                                 "682e6ff3",
                                 "748f82ee", "78a5636f", "84c87814", "8cc70208", "90befffa", "a4506ceb", "bef9a3f7",
                                 "c67178f2"
    };

    for (int i = 0; i < N; i++) {

        a = pre_a;
        b = pre_b;
        c = pre_c;
        d = pre_d;
        e = pre_e;
        f = pre_f;
        g = pre_g;
        h = pre_h;

        vector<Block> w = expansion(blocks[i]);

        for (int j = 0; j < 64; j++) {
            vector<int> chv = ch(e, f, g);
            vector<int> majv = maj(a, b, c);
            vector<int> sg0 = bigSimga0(a);
            vector<int> sg1 = bigSimga1(e);
            vector<int> sg2 = bigSimga2(add(c, d));
            vector<int> k_int = bitsetToVecInt(bitset<32>(hexToBin(k[j])));
            t2 = add(add(add(h, sg1), chv), add(k_int, w[j].data));
            t1 = add(add(sg0, majv), sg2);
            h = g;
            f = e;
            d = c;
            b = a;
            g = f;
            e = add(d, t1);
            c = b;
            vector<int> tempo = multiply(3, t1);
            a = sub(tempo, t2);

//        standard :

//        t1 = add(add(add(h,sg1),chv ), add(k_int, w[j].data)     );
//        t2 = add( sg0 ,majv  );
//        h=g;
//        g=f;
//        f=e;
//        e=add(d,t1);
//        d=c;
//        c=b;
//        b=a;
//        a =add(t1,t2);

        }

//         updating hash values

        pre_a = add(a, pre_a);
        pre_b = add(b, pre_b);
        pre_c = add(c, pre_c);
        pre_d = add(d, pre_d);
        pre_e = add(e, pre_e);
        pre_f = add(f, pre_f);
        pre_g = add(g, pre_g);
        pre_h = add(h, pre_h);
    }

    a = pre_a;
    b = pre_b;
    c = pre_c;
    d = pre_d;
    e = pre_e;
    f = pre_f;
    g = pre_g;
    h = pre_h;

    string res = binToHex(a);
    res.append(binToHex(b));
    res.append(binToHex(c));
    res.append(binToHex(d));
    res.append(binToHex(e));
    res.append(binToHex(f));
    res.append(binToHex(g));
    res.append(binToHex(h));

    return res;
}

vector<int> bigSimga0(vector<int> x) {

    vector<int> result, temp1, temp2;
    temp1 = XOR(ROT(x, 2), ROT(x, 13));
    temp2 = XOR(ROT(x, 22), SHF(x, 7));
    result = XOR(temp1, temp2);


//        sha-256 standard :
//    temp1 = XOR(ROT(x, 2), ROT(x, 13));//   result = XOR(temp1, ROT(x, 22));

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

//    standard :
//    res= XOR(andGate(x, y), andGate(notGate(x), z));


    return res;
}

vector<int> maj(vector<int> x, vector<int> y, vector<int> z) {

    vector<int> temp1, res;
    temp1 = XOR(andGate(x, z), andGate(x, y));

    res = XOR(temp1, andGate(y, z));

    return res;
}

vector<int> andGate(vector<int> x, vector<int> y) {


    vector<int> result;

    int minSize = min(x.size(), y.size());
    std::reverse(x.begin(), x.end());
    std::reverse(y.begin(), y.end());

    for (int i = 0; i < minSize; i++) {

        result.push_back(x[i] & y[i]);
    }

    std::reverse(result.begin(), result.end());
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

    std::reverse(res.begin(), res.end());
    return res;

}

vector<int> bitsetToVecInt(bitset<8> b) {

    vector<int> res;
    for (int i = 0; i < b.size(); i++) {
        res.push_back((int) b[i]);

    }
    std::reverse(res.begin(), res.end());
    return res;

}

vector<int> bitsetToVecInt(bitset<256> b) {

    vector<int> res;
    for (int i = 0; i < b.size(); i++) {
        res.push_back((int) b[i]);

    }
    std::reverse(res.begin(), res.end());
    return res;
}

vector<int> bitsetToVecInt(bitset<640> b) {

    vector<int> res;
    for (int i = 0; i < b.size(); i++) {
        res.push_back((int) b[i]);

    }
    std::reverse(res.begin(), res.end());
    return res;
}

vector<int> bitsetToVecInt(bitset<64> b) {

    vector<int> res;
    for (int i = 0; i < b.size(); i++) {
        res.push_back((int) b[i]);

    }
    std::reverse(res.begin(), res.end());
    return res;

}

vector<int> strToBinary(string txt) {

    vector<int> result;
    for (int i = 0; i < txt.size(); i++) {

        vector<int> tmp = bitsetToVecInt(bitset<8>(txt[i]));

        for (int j = 0; j < tmp.size(); j++) {
            result.push_back(tmp[j]);
        }
    }
    return result;

}

vector<int> multiply(int n, vector<int> x) {

    vector<int> res = x;

    for (int i = 0; i < n - 1; ++i) {

        res = add(res, x);

    }
    return res;

}

vector<int> sub(vector<int> x, vector<int> y) {
    vector<int> res;
    vector<int> one;
    for (int i = 0; i < y.size() - 1; i++)
        one.push_back(0);
    one.push_back(1);

    res = add(x, add(notGate(y), one));
//    res = add(x,notGate(y));

    return res;
}

vector<int> updateNumber(int size, vector<int> number) {

    while (number.size() <= size) {

        number.insert(number.begin(), 0);

    }


    return number;
}

vector<int> stringToIntVector(string text){
    vector<int> result;
    for (int i = 0; i < text.size(); i++) {
        switch (text[i]){
            case '0':
                result.push_back(0);
                break;
            case '1':
                result.push_back(1);
                break;
        }
    }
    return result;
}

bool lessThan(vector<int> b1, vector<int> b2) {

    bool result = false;

    for (int i = 0; i < b1.size(); i++) {
        if (b1[i] > b2[i]) {
            result = false;
            break;
        }
        else {
            if (b1[i] < b2[i]) {
                result = true;
                break;
            }
        }
    }
    return result;
}

vector<int> updateBlockHeader(vector<int> block_header, vector<int> nonce){

    string blockHeader = "";
    for (int i = 0; i < block_header.size(); ++i) {
        blockHeader.append(intToChar(block_header[i]));
    }

    string nonceStr = "";
    for (int i = 0; i < nonce.size(); ++i) {
        nonceStr.append(intToChar(nonce[i]));
    }

    string result = "";
    result.append(nonceStr);
    result.append(blockHeader);

    return stringToIntVector(result);
}