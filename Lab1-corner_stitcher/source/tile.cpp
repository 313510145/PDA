#include "tile.h"

int tile::get_index() const {
    return this->index;
}

int tile::get_lower_left_x() const {
    return this->lower_left.x;
}

int tile::get_lower_left_y() const {
    return this->lower_left.y;
}

coordinate tile::get_lower_left() const {
    return this->lower_left;
}

int tile::get_upper_right_x() const {
    return this->upper_right.x;
}

int tile::get_upper_right_y() const {
    return this->upper_right.y;
}

coordinate tile::get_upper_right() const {
    return this->upper_right;
}

void tile::set_lower_left_x(int llx) {
    this->lower_left.x = llx;
}

void tile::set_lower_left_y(int lly) {
    this->lower_left.y = lly;
}

void tile::set_upper_right_x(int urx) {
    this->upper_right.x = urx;
}

void tile::set_upper_right_y(int ury) {
    this->upper_right.y = ury;
}

tile* tile::get_left_bottom() const {
    return this->left_bottom;
}

tile* tile::get_bottom_left() const {
    return this->bottom_left;
}

tile* tile::get_right_top() const {
    return this->right_top;
}

tile* tile::get_top_right() const {
    return this->top_right;
}

void tile::set_left_bottom(tile* lb) {
    this->left_bottom = lb;
}

void tile::set_bottom_left(tile* bl) {
    this->bottom_left = bl;
}

void tile::set_right_top(tile* rt) {
    this->right_top = rt;
}

void tile::set_top_right(tile* tr) {
    this->top_right = tr;
}

tile::tile() {
    this->index = 0;
    this->lower_left.x = 0;
    this->lower_left.y = 0;
    this->upper_right.x = 0;
    this->upper_right.y = 0;
    this->left_bottom = nullptr;
    this->bottom_left = nullptr;
    this->right_top = nullptr;
    this->top_right = nullptr;
}

tile::tile(int index_in, coordinate lower_left_in, coordinate upper_right_in) {
    this->index = index_in;
    this->lower_left = lower_left_in;
    this->upper_right = upper_right_in;
    this->left_bottom = nullptr;
    this->bottom_left = nullptr;
    this->right_top = nullptr;
    this->top_right = nullptr;
}

tile::~tile() {}
