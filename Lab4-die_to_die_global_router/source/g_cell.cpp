#include "g_cell.h"

int g_cell::get_horizontal_capacity() const {
    return this->horizontal_capacity;
}

int g_cell::get_vertical_capacity() const {
    return this->vertical_capacity;
}

double g_cell::get_horizontal_cost() const {
    return this->horizontal_cost;
}

double g_cell::get_vertical_cost() const {
    return this->vertical_cost;
}

int g_cell::get_left_overflow() const {
    if (this->left_net_list.size() < this->horizontal_capacity) {
        return 0;
    }
    return (this->left_net_list.size() + 1 - this->horizontal_capacity);
}

int g_cell::get_right_overflow() const {
    if (this->right_net_list.size() < this->horizontal_capacity) {
        return 0;
    }
    return (this->right_net_list.size() + 1 - this->horizontal_capacity);
}

int g_cell::get_top_overflow() const {
    if (this->top_net_list.size() < this->vertical_capacity) {
        return 0;
    }
    return (this->top_net_list.size() + 1 - this->vertical_capacity);
}

int g_cell::get_bottom_overflow() const {
    if (this->bottom_net_list.size() < this->vertical_capacity) {
        return 0;
    }
    return (this->bottom_net_list.size() + 1 - this->vertical_capacity);
}

void g_cell::set_horizontal_capacity(unsigned int horizontal_capacity_in) {
    this->horizontal_capacity = horizontal_capacity_in;
}

void g_cell::set_vertical_capacity(unsigned int vertical_capacity_in) {
    this->vertical_capacity = vertical_capacity_in;
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
    this->horizontal_capacity = 0;
    this->vertical_capacity = 0;
    this->horizontal_cost = 0;
    this->vertical_cost = 0;
}

g_cell::~g_cell() {
    this->left_net_list.clear();
    this->right_net_list.clear();
    this->top_net_list.clear();
    this->bottom_net_list.clear();
}
