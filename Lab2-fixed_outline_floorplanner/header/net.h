#ifndef NET_H
#define NET_H

#include "block.h"
#include <vector>

class net {
    public:
        block* get_block(unsigned int i) const;
        unsigned int get_block_list_size() const;
        void add_block(block* b_in);
        net();
        ~net();
    private:
        std::vector<block*> block_list;
};

#endif  // NET_H
