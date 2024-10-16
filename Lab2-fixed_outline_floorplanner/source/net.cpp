#include "net.h"

block* net::get_block(unsigned int i) const {
    if (i < this->block_list.size()) {
        return this->block_list[i];
    }
    return nullptr;
}

unsigned int net::get_block_list_size() const {
    return this->block_list.size();
}

void net::add_block(block* b_in) {
    if (b_in != nullptr) {
        this->block_list.push_back(b_in);
    }
}

void net::clear_block_list() {
    this->block_list.clear();
}

net::net() {}

net::~net() {}
