#ifndef CORNER_STITCHER_H
#define CORNER_STITCHER_H

#include "tile.h"
#include <iostream>
#include <vector>

const int SPACE_INDEX = 0;

class corner_stitcher {
    public:
        void input_information(std::istream& is);
        void output_layout(std::ostream& os) const;
        void output_tile_list_neighbor_num(std::ostream& os);
        void output_point_finding(std::ostream& os) const;
        corner_stitcher();
        ~corner_stitcher();
    private:
        void point_finding_to_buffer(const coordinate& p);
        tile* point_finding(const coordinate& p);
        void block_creating(const int& i, coordinate& ll, coordinate& a);
        void horizontal_cut(tile* s, const int& nb_ur_y);
        void vertical_cut(tile* s, tile* b);
        void merge(tile* bs);
        void delete_space_in_list(tile* s);
        static void neighbor_count(const tile* b, int& bn, int& sn);
        coordinate area;
        std::vector<tile*> tile_list;
        std::vector<coordinate> output_buffer;
};

#endif  // CORNER_STITCHER_H
