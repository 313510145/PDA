#ifndef DIE_TO_DIE_GLOBAL_ROUTER_H
#define DIE_TO_DIE_GLOBAL_ROUTER_H

#include "g_cell.h"
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

enum DIRECTION {LEFT, RIGHT, UP, DOWN, DIRECTION_NUM};
const int REVERSE_DIRECTION[] = {RIGHT, LEFT, DOWN, UP};

struct xy {
    int x, y;
    xy() {
        this->x = 0;
        this->y = 0;
    }
    xy(int x_in, int y_in) {
        this->x = x_in;
        this->y = y_in;
    }
};

struct node {
    xy coordinate;
    double f, g;
    node() {
        this->f = 0;
        this->g = 0;
    }
    node(int x_in, int y_in): coordinate(x_in, y_in) {
        this->f = 0;
        this->g = 0;
    }
    bool operator<(const node& a) const {
        return this->f > a.f;
    }
};

class die_to_die_global_router {
    public:
        void input_map(std::istream& is);
        void input_g_cell(std::istream& is);
        void input_cost(std::istream& is);
        void a_star_search();
        void output_results(std::ostream& os);
        die_to_die_global_router();
        ~die_to_die_global_router();
    private:
        double lower_left_x, lower_left_y;
        int grid_x_num, grid_y_num;
        double grid_width, grid_height;
        std::vector<std::vector<g_cell>> grid_map;
        std::map<int, std::pair<xy, xy>> net_list;
        std::unordered_map<int, std::vector<xy>> net_list_path;
        double alpha, beta, gamma, delta;
        double via_cost;
        double max_horizontal_cell_cost, max_vertical_cell_cost;
};

#endif  // DIE_TO_DIE_GLOBAL_ROUTER_H
