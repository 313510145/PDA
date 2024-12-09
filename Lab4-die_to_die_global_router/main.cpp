#include "die_to_die_global_router.h"
#include <fstream>

int main(int argc, char** argv) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " *.gmp *.gcl *.cst *.lg\n";
        exit(-1);
    }
    std::ifstream input_file(argv[1]);
    if (!input_file) {
        std::cout << "Cannot open the input file: " << argv[1] << std::endl;
        exit(-1);
    }
    die_to_die_global_router* dtdgr = new die_to_die_global_router();
    dtdgr->input_map(input_file);
    input_file.close();
    input_file.open(argv[2]);
    if (!input_file) {
        std::cout << "Cannot open the input file: " << argv[2] << std::endl;
        delete dtdgr;
        exit(-1);
    }
    dtdgr->input_g_cell(input_file);
    input_file.close();
    input_file.open(argv[3]);
    if (!input_file) {
        std::cout << "Cannot open the input file: " << argv[3] << std::endl;
        delete dtdgr;
        exit(-1);
    }
    dtdgr->input_cost(input_file);
    input_file.close();
    dtdgr->a_star_search();
    std::ofstream output_file(argv[4]);
    if (!output_file) {
        std::cout << "Cannot open the output file: " << argv[4] << std::endl;
        delete dtdgr;
        exit(-1);
    }
    dtdgr->output_results(output_file);
    delete dtdgr;
    output_file.close();
    return 0;
}
