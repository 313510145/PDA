#include "g_cell.h"

unsigned int g_cell::get_left_capacity() const {
    return this->left_capacity;
}

unsigned int g_cell::get_right_capacity() const {
    return this->right_capacity;
}

unsigned int g_cell::get_top_capacity() const {
    return this->top_capacity;
}

unsigned int g_cell::get_bottom_capacity() const {
    return this->bottom_capacity;
}

double g_cell::get_horizontal_cost() const {
    return this->horizontal_cost;
}

double g_cell::get_vertical_cost() const {
    return this->vertical_cost;
}

unsigned int g_cell::get_left_overflow() const {
    if (this->left_net_list.size() < this->left_capacity) {
        return 0;
    }
    return (this->left_net_list.size() + 1 - this->left_capacity);
}

unsigned int g_cell::get_right_overflow() const {
    if (this->right_net_list.size() < this->right_capacity) {
        return 0;
    }
    return (this->right_net_list.size() + 1 - this->right_capacity);
}

unsigned int g_cell::get_top_overflow() const {
    if (this->top_net_list.size() < this->top_capacity) {
        return 0;
    }
    return (this->top_net_list.size() + 1 - this->top_capacity);
}

unsigned int g_cell::get_bottom_overflow() const {
    if (this->bottom_net_list.size() < this->bottom_capacity) {
        return 0;
    }
    return (this->bottom_net_list.size() + 1 - this->bottom_capacity);
}

void g_cell::set_left_capacity(unsigned int left_capacity_in) {
    this->left_capacity = left_capacity_in;
}

void g_cell::set_right_capacity(unsigned int right_capacity_in) {
    this->right_capacity = right_capacity_in;
}

void g_cell::set_top_capacity(unsigned int top_capacity_in) {
    this->top_capacity = top_capacity_in;
}

void g_cell::set_bottom_capacity(unsigned int bottom_capacity_in) {
    this->bottom_capacity = bottom_capacity_in;
}

void g_cell::set_horizontal_cost(double horizontal_cost_in) {
    this->horizontal_cost = horizontal_cost_in;
}

void g_cell::set_vertical_cost(double vertical_cost_in) {
    this->vertical_cost = vertical_cost_in;
}

void g_cell::add_net_to_left(int i) {
    this->left_net_list.insert(i);
}

void g_cell::add_net_to_right(int i) {
    this->right_net_list.insert(i);
}

void g_cell::add_net_to_top(int i) {
    this->top_net_list.insert(i);
}

void g_cell::add_net_to_bottom(int i) {
    this->bottom_net_list.insert(i);
}

g_cell::g_cell() {
    this->left_capacity = 0;
    this->right_capacity = 0;
    this->top_capacity = 0;
    this->bottom_capacity = 0;
    this->horizontal_cost = 0;
    this->vertical_cost = 0;
}

g_cell::~g_cell() {
    this->left_net_list.clear();
    this->right_net_list.clear();
    this->top_net_list.clear();
    this->bottom_net_list.clear();
}
