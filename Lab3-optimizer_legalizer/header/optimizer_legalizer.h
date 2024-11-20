#ifndef OPTIMIZER_LEGALIZER_H
#define OPTIMIZER_LEGALIZER_H

#include "block.h"
#include "segment.h"
#include <iostream>
#include <vector>
#include <map>

const double DEFAULT_WIDTH = 1.0;

class optimizer_legalizer {
    public:
        void input_site(std::istream& is);
        void input_merged_block_and_output_result(std::istream& is, std::ostream& os);
        optimizer_legalizer();
        ~optimizer_legalizer();
    private:
        void find_segments_and_cut(block* b_in);
        void create_segments_and_merge(block* b_in);
        double alpha, beta;
        double lower_left_x, lower_left_y;
        double upper_right_x, upper_right_y;
        std::map<std::string, block*> block_name_map;
        std::map<double, std::vector<segment*>> segment_map;
};

#endif  // OPTIMIZER_LEGALIZER_H
