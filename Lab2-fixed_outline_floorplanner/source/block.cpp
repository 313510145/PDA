#include "block.h"

bool block::get_flag() const {
    return this->flag;
}

std::string block::get_name() const {
    return this->name;
}

int block::get_lower_left_x() const {
    return this->lower_left.x;
}

int block::get_lower_left_y() const {
    return this->lower_left.y;
}

int block::get_upper_right_x() const {
    return this->upper_right.x;
}

int block::get_upper_right_y() const {
    return this->upper_right.y;
}

int block::get_middle_x() const {
    return ((this->upper_right.x + this->lower_left.x) / 2);
}

int block::get_middle_y() const {
    return ((this->upper_right.y + this->lower_left.y) / 2);
}

unsigned int block::get_negative_index() const {
    return this->negative_index;
}

void block::set_flag() {
    this->flag = true;
}

void block::reset_flag() {
    this->flag = false;
}

void block::set_lower_left_x(int x_in) {
    this->lower_left.x = x_in;
}

void block::set_lower_left_y(int y_in) {
    this->lower_left.y = y_in;
}

void block::set_upper_right_x(int x_in) {
    this->upper_right.x = x_in;
}

void block::set_upper_right_y(int y_in) {
    this->upper_right.y = y_in;
}

void block::set_negative_index(unsigned int negative_index_in) {
    this->negative_index = negative_index_in;
}

block::block() {
    this->flag = false;
    this->name = "";
    this->lower_left.x = 0;
    this->lower_left.y = 0;
    this->upper_right.x = 0;
    this->upper_right.y = 0;
    this->negative_index = 0;
}

block::block(bool is_terminal_in, const std::string& name_in, int x_in, int y_in) {
    this->flag = false;
    this->name = name_in;
    if (is_terminal_in == true) {
        this->lower_left.x = x_in;
        this->lower_left.y = y_in;
        this->upper_right.x = x_in;
        this->upper_right.y = y_in;
    }
    else {
        this->lower_left.x = 0;
        this->lower_left.y = 0;
        this->upper_right.x = x_in;
        this->upper_right.y = y_in;
    }
    this->negative_index = 0;
}

block::~block() {}
