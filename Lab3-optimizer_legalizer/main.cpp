#include "optimizer_legalizer.h"
#include <fstream>

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " *.lg *.opt *_post.lg\n";
        exit(-1);
    }
    std::ifstream input_file(argv[1]);
    if (!input_file) {
        std::cout << "Cannot open the input file: " << argv[1] << std::endl;
        exit(-1);
    }
    optimizer_legalizer* ol = new optimizer_legalizer();
    ol->input_site(input_file);
    input_file.close();
    input_file.open(argv[2]);
    if (!input_file) {
        std::cout << "Cannot open the input file: " << argv[2] << std::endl;
        delete ol;
        exit(-1);
    }
    std::ofstream output_file(argv[3]);
    if (!output_file) {
        std::cout << "Cannot open the output file: " << argv[3] << std::endl;
        delete ol;
        exit(-1);
    }
    ol->input_merged_block_and_output_result(input_file, output_file);
    delete ol;
    input_file.close();
    output_file.close();
    return 0;
}
