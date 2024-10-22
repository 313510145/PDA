#ifndef FIXED_OUTLINE_FLOORPLANNER_H
#define FIXED_OUTLINE_FLOORPLANNER_H

#include "block.h"
#include "net.h"
#include <iostream>
#include <vector>

const double MAX_TEMPERATURE = 1e7;
const double MIN_TEMPERATURE = 1e-5;
const double REDUCTION_RATE = 1 - 1e-3 - 1e-5;
enum OPERATION {SWAP_IN_POSITIVE, SWAP_IN_NEGATIVE, SWAP_IN_BOTH, ROTATE, OPERATION_NUM};

class fixed_outline_floorplanner {
    public:
        void input_block(std::istream& is);
        void input_net(std::istream& is);
        void simulated_annealing();
        void output_report(std::ostream& os) const;
        fixed_outline_floorplanner();
        explicit fixed_outline_floorplanner(char* alpha_in);
        ~fixed_outline_floorplanner();
    private:
        void initialize_sequence_pair();
        void swap_in_positive(unsigned int a, unsigned int b);
        void swap_in_negative(unsigned int a, unsigned int b);
        void swap_in_both(unsigned int a, unsigned int b);
        void rotate(unsigned int i);
        void place_in();
        void calculate_area();
        void calculate_extra_area();
        void calculate_cost();
        void choice_and_index_operation(unsigned int choice, unsigned int a, unsigned int b);
        double temperature;
        double run_time;
        double alpha;
        int cost, best_cost;
        coordinate outline;
        coordinate area;
        int area_value, best_area_value;
        int hpwl;
        std::vector<block*> block_list;
        std::vector<net*> net_list;
        std::vector<block*> sequence_pair_positive, sequence_pair_negative;
};

#endif  // FIXED_OUTLINE_FLOORPLANNER_H
