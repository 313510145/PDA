#include "corner_stitcher.h"
#include <fstream>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <debugging_file-optional>\n";
        exit(-1);
    }
    std::ifstream input_file(argv[1]);
    if (!input_file) {
        std::cout << "Cannot open the input file: " << argv[1] << std::endl;
        exit(-1);
    }
    corner_stitcher* cs = new corner_stitcher();
    cs->input_information(input_file);
    input_file.close();
    std::ofstream output_file(argv[2]);
    if (!output_file) {
        std::cout << "Cannot open the output file: " << argv[2] << std::endl;
        delete cs;
        exit(-1);
    }
    if (argc == 4) {
        std::ofstream debugging_file(argv[3]);
        if (!debugging_file) {
            std::cout << "Cannot open the debugging file: " << argv[3] << std::endl;
            delete cs;
            exit(-1);
        }
        cs->output_layout(debugging_file);
        debugging_file.close();
    }
    cs->output_tile_list_neighbor_num(output_file);
    cs->output_point_finding(output_file);
    delete cs;
    output_file.close();
    return 0;
}
