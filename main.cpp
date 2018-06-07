#include "library/huffman.h"

#include <iostream>
#include <fstream>
#include <algorithm>

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "Usage: huffman [-d] input_file [output_file]" << std::endl;
        return 0;
    }

    bool decode = false;
    if (std::string(argv[1]) == "-d") {
        decode = true;
    }

    std::ifstream input(argv[1 + decode], std::ios::binary);
    std::ofstream output(argv[2 + decode], std::ios::binary);

    if (decode) {
        huffman::read_encoded(input, output);
    } else {
        std::vector<size_t> counts(256);
        unsigned char ch;
        size_t cnt = 0;
        while (input >> std::noskipws >> ch) {
            counts[ch]++;
            cnt++;
        }

        std::vector<std::pair<size_t, unsigned char>> chars;
        ch = 0;
        do {
            if (counts[ch] != 0) { chars.emplace_back(counts[ch], ch); }
        } while (++ch != 0);

        std::shared_ptr<huffman::tree> tree = huffman::build_tree(chars);

        input.clear();
        input.seekg(0);

        huffman::write_encoded(tree, cnt, input, output);
    }
    return 0;
}