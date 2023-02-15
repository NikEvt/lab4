#include <iostream>
#include <regex>
#include "hammcode.h"

int main() {
//    hammcode::hammingCode("4.txt");
//    hammcode::hammingCode("1.txt");
    hammcode::concat({"1.txt", "4.txt"}, "arc.haf");
    //hammcode::extract("4.txt", "arc.haf");
    //hammcode::deleteFile("4.txt", "arc.haf");
    return 0;
}