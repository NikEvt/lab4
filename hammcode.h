//
// Created by th3me on 29.01.2023.
//

#ifndef LABWORK4_ARCHIVER_FINAL_HAMMCODE_H
#define LABWORK4_ARCHIVER_FINAL_HAMMCODE_H
#include <iostream>
#include <vector>

namespace hammcode {

    [[maybe_unused]] void hammingCode(const std::string &fileName);

    [[maybe_unused]] void fillControlBits(std::vector<bool> &bits);

    [[maybe_unused]] int pow(int a, int power);

    [[maybe_unused]] void checkControlBits(std::vector<bool> &bits);

    [[maybe_unused]] void decoding(const std::string &arcName);

    [[maybe_unused]] void concat(const std::vector<std::string> &arcs, const std::string &arcName);

    [[maybe_unused]] void deleteFile(const std::string &fileName, const std::string &arcName);

    [[maybe_unused]] void concatenateArchives(const std::vector<std::string> &vec_arcs, const std::string &arcName);

    [[maybe_unused]] void extract(const std::string &fileName);

    [[maybe_unused]] void extract(const std::string &fileName, const std::string &arcName);

}

#endif //LABWORK4_ARCHIVER_FINAL_HAMMCODE_H
