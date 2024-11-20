#include "segment.h"
#include <float.h>

double segment::get_left_x() const {
    return this->left_x;
}

double segment::get_right_x() const {
    return this->right_x;
}

double segment::get_width() const {
    return this->width;
}

void segment::set_left_x(double left_x_in) {
    this->left_x = left_x_in;
}

void segment::set_right_x(double right_x_in) {
    this->right_x = right_x_in;
}

void segment::set_width(double width_in) {
    this->width = width_in;
}

segment::segment() {
    this->left_x = DBL_MIN;
    this->right_x = DBL_MAX;
    this->width = 0;
}

segment::~segment() {}
