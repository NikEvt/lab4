
#include "hammcode.h"
#include <iostream>
#include <fstream>

[[maybe_unused]] void hammcode::hammingCode(const std::string &fileName) {
    std::ifstream f(fileName, std::ios::binary);
    std::ofstream fout("tmp.haf", std::ios::binary);
    std::vector<bool> bits(8);
    char c;
    while (f.get(c)) {
        int p = 7;
        int deg2 = 1;
        for (int i = 0; i < 8; ++i) {
            if (i == deg2 - 1) {
                bits[i] = false;
                deg2 *= 2;
            } else {
                bits[i] = (c / pow(2, p)) % 2;
                p--;
            }
        }
        hammcode::fillControlBits(bits);
        char cur = 0;

        for (int i = 0; i < 8; ++i)
            cur += hammcode::pow(2, 7 - i) * bits[i];

        char q[1] = {cur};
        fout.write(q, 1);
        deg2 = 1;
        for (int i = 0; i < 8; ++i) {
            if (i == deg2 - 1) {
                bits[i] = false;
                deg2 *= 2;
            } else {
                bits[i] = (c / hammcode::pow(2, p)) % 2;
                p--;
            }
        }
        fillControlBits(bits);
        checkControlBits(bits);
        cur = 0;
        for (int i = 0; i < 8; ++i) {
            cur += hammcode::pow(2, 7 - i) * bits[i];
        }
        q[0] = cur;
        fout.write(q, 1);
    }
    fout.close();
    f.close();
    remove(fileName.c_str());
    rename("tmp.haf", fileName.c_str());
}

void hammcode::fillControlBits(std::vector<bool> &bits) {
    int sumOfBits = 0;
    for (int i = 1; i < 8; i *= 2) {
        int it = i * 2;
        int tmpSum = 0;
        for (int j = i - 1; j < 8; ++j) {
            if ((j - i + 1) % it < i) {
                tmpSum += bits[j];
            }
        }
        bits[i - 1] = tmpSum % 2;
    }
    for (int i = 0; i < 8; i++) {
        sumOfBits += bits[i];
    }
    bits[7] = sumOfBits % 2;
}

int hammcode::pow(int a, int power) {
    int res = 1;
    while (power > 0) {
        if (power % 2 == 0) {
            power /= 2;
            a *= a;
        } else {
            power -= 1;
            res = res * a;
            power /= 2;
            a *= a;
        }
    }
    return res;
}

void hammcode::checkControlBits(std::vector<bool> &bits) {
    int parityBits[3] = {0, 0, 0};
    int id = 0;
    for (int i = 1; i <= 4; i *= 2) {
        for (int j = i - 1; j < 8; j += i * 2) {
            for (int k = j; k < j + i && k < 8; ++k) {
                parityBits[id] += bits[k];
            }
        }
        ++id;
    }
    int errorBit = (parityBits[0] % 2) * 1 + (parityBits[1] % 2) * 2 + (parityBits[2] % 2) * 4;
    if (errorBit != 0) {
        std::cout << "Error detected at bit " << errorBit << std::endl;
        bits[errorBit - 1] = !bits[errorBit - 1];
    }
}

void hammcode::decoding(const std::string &arcName) {
    std::ifstream f(arcName, std::ios::binary);
    std::ofstream fout("tmp.haf", std::ios::binary);
    char c;
    char h;
    std::vector<bool> bits1;
    std::vector<bool> bits2;
    std::vector<bool> decodedBits;
    while (f.get(c)) {
        f.get(h);
        for (int i = 7; i >= 0; --i)
            bits1.push_back(((unsigned char) c / hammcode::pow(2, i)) % 2);

        for (int i = 7; i > -1; --i)
            bits2.push_back(((unsigned char) h / hammcode::pow(2, i)) % 2);

        decodedBits.push_back(bits1[2]);
        decodedBits.push_back(bits1[4]);
        decodedBits.push_back(bits1[5]);
        decodedBits.push_back(bits1[6]);
        decodedBits.push_back(bits2[2]);
        decodedBits.push_back(bits2[4]);
        decodedBits.push_back(bits2[5]);
        decodedBits.push_back(bits2[6]);
        {
        }
        char cur[1] = {0};
        for (int i = 0; i < 8; ++i) {
            cur[0] += hammcode::pow(2, 7 - i) * decodedBits[i];
        }
        decodedBits.clear();
        bits1.clear();
        bits2.clear();
        fout.write(cur, 1);
    }
    fout.close();
    f.close();
    remove(arcName.c_str());
    rename("tmp.haf", arcName.c_str());
}

void hammcode::concat(const std::vector<std::string> &arcs, const std::string &arcName) {
    std::ofstream result_archive(arcName, std::ios::binary);
    for (auto curFile: arcs) {
        std::ifstream curArc(curFile, std::ios::binary);
        for (auto i: curFile) {
            char q[1] = {i};
            result_archive.write(q, 1);
        }
        result_archive.write("\n", 1);
        char c[1];
        while (curArc.get(c[0])) {
            result_archive.write(c, 1);
        }
        result_archive.write("\n", 1);
    }
}


void hammcode::deleteFile(const std::string &fileName, const std::string &arcName) {
    std::ifstream archive(arcName, std::ios::binary);
    int strNum = 0;
    bool flag = false;
    while (!archive.eof()) {
        if (strNum % 2 == 0) {
            char c;
            std::string curFile;
            int pos = archive.tellg();
            int pos2;
            while (archive.get(c)) {
                if (c != '\n')
                    curFile += c;
                else
                    break;
            }
            std::cout << curFile;
            if (curFile == fileName) {
                flag = true;
                while (!archive.eof()) {
                    archive.get(c);
                    if (c != '\n') {
                        continue;
                    } else {
                        pos2 = archive.tellg();
                        break;
                    }
                }
                std::ofstream newArc("tmp.haf", std::ios::binary);
                archive.seekg(std::ios::beg);
                char q[1];
                while (archive.tellg() != pos) {
                    archive.get(q[0]);
                    newArc.write(q, 1);
                }
                archive.seekg(pos2, std::ios::beg);
                while (!archive.eof()) {
                    archive.get(q[0]);
                    newArc.write(q, 1);
                }
                newArc.close();
            }
        } else {
            strNum += 2;
            char c;
            while (archive.eof()) {
                archive.get(c);
                if (c == '\n') {
                    break;
                }
            }
        }
    }
    archive.close();
        remove(arcName.c_str());
        rename("tmp.haf", arcName.c_str());
}


void hammcode::concatenateArchives(const std::vector<std::string> &vec_arcs, const std::string &arcName) {
    std::ofstream result_archive(arcName, std::ios::binary);
    for (auto s: vec_arcs) {
        std::ifstream curArc(s, std::ios::binary);
        char c[1];
        while (curArc.get(c[0])) {
            result_archive.write(c, 1);
        }
    }
}

void hammcode::extract(const std::string &fileName) {
    int strNum = 0;
    std::ifstream archive(fileName, std::ios::binary);
    char c[1];
    std::string file;
    while (archive.get(c[0])) {
        if (c[0] == '\n')
            strNum++;
        if (strNum == 0) {
            file += c[0];
        }
        if (strNum == 1) {
            std::ofstream curFile(file, std::ios::binary);
            while (archive.get(c[0])) {
                if (c[0] == '\n')
                    break;
                curFile.write(c, 1);
            }
            curFile.close();
            hammcode::decoding(file);
            strNum = 0;
            file.clear();
        }
    }
    remove(fileName.c_str());
}

void hammcode::extract(const std::string &fileName, const std::string &arcName) {
    std::ifstream archive(arcName, std::ios::binary);
    char c[1];
    std::string curFile;
    while (!archive.eof()) {
        while (archive.get(c[0])) {
            if (c[0] == '\n') {
                if(curFile == fileName){
                    while(archive.get(c[0])){
                        std::ofstream newArc(fileName, std::ios::binary);
                        newArc.write(c, 1);
                        if(c[0] == '\n'){
                            newArc.close();
                            //decoding(fileName);
                            break;
                        }
                    }
                } else {
                    curFile.clear();
                }
                while(archive.get(c[0])){
                    if(c[0] == '\n'){
                        break;
                    }
                }
            }
            if(c[0] != '\n') {
                curFile += c[0];
            }
        }
    }
    archive.close();
    deleteFile(fileName, arcName);
    return;
}

