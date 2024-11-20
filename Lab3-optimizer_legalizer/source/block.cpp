#include "block.h"

std::string block::get_name() const {
    return this->name;
}

double block::get_lower_left_x() const {
    return this->lower_left_x;
}

double block::get_lower_left_y() const {
    return this->lower_left_y;
}

double block::get_upper_right_x() const {
    return this->upper_right_x;
}

double block::get_upper_right_y() const {
    return this->upper_right_y;
}

double block::get_width() const {
    return (this->upper_right_x - this->lower_left_x);
}

double block::get_height() const {
    return (this->upper_right_y - this->lower_left_y);
}

bool block::get_fixed() const {
    return this->fixed;
}

void block::set_name(const std::string& name_in) {
    this->name = name_in;
}

void block::set_lower_left_x(double lower_left_x_in) {
    this->lower_left_x = lower_left_x_in;
}

void block::set_lower_left_y(double lower_left_y_in) {
    this->lower_left_y = lower_left_y_in;
}

void block::set_upper_right_x(double upper_right_x_in) {
    this->upper_right_x = upper_right_x_in;
}

void block::set_upper_right_y(double upper_right_y_in) {
    this->upper_right_y = upper_right_y_in;
}

void block::set_fixed(bool fixed_in) {
    this->fixed = fixed_in;
}

block::block() {
    this->name = "";
    this->lower_left_x = 0;
    this->lower_left_y = 0;
    this->upper_right_x = 0;
    this->upper_right_y = 0;
    this->fixed = 0;
}

block::~block() {}
