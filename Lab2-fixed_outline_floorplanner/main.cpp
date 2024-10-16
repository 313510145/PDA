#include "fixed_outline_floorplanner.h"
#include <fstream>

int main(int argc, char** argv) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " <alpha_value> <input_block> <input_net> <output_file>\n";
        exit(-1);
    }
    fixed_outline_floorplanner* fof = new fixed_outline_floorplanner(argv[1]);
    std::ifstream input_file(argv[2]);
    if (!input_file) {
        std::cout << "Cannot open the input file: " << argv[2] << std::endl;
        exit(-1);
    }
    fof->input_block(input_file);
    input_file.close();
    input_file.open(argv[3]);
    if (!input_file) {
        std::cout << "Cannot open the input file: " << argv[3] << std::endl;
        exit(-1);
    }
    fof->input_net(input_file);
    input_file.close();
    fof->random_search();
    std::ofstream output_file(argv[4]);
    if (!output_file) {
        std::cout << "Cannot open the output file: " << argv[4] << std::endl;
        delete fof;
        exit(-1);
    }
    fof->output_report(output_file);
    delete fof;
    output_file.close();
    return 0;
}
