#ifndef G_CELL_H
#define G_CELL_H

#include <unordered_set>

class g_cell {
    public:
        unsigned int get_left_capacity() const;
        unsigned int get_right_capacity() const;
        unsigned int get_top_capacity() const;
        unsigned int get_bottom_capacity() const;
        double get_horizontal_cost() const;
        double get_vertical_cost() const;
        unsigned int get_left_overflow() const;
        unsigned int get_right_overflow() const;
        unsigned int get_top_overflow() const;
        unsigned int get_bottom_overflow() const;
        void set_left_capacity(unsigned int left_capacity_in);
        void set_right_capacity(unsigned int right_capacity_in);
        void set_top_capacity(unsigned int top_capacity_in);
        void set_bottom_capacity(unsigned int bottom_capacity_in);
        void set_horizontal_cost(double horizontal_cost_in);
        void set_vertical_cost(double vertical_cost_in);
        void add_net_to_left(int i);
        void add_net_to_right(int i);
        void add_net_to_top(int i);
        void add_net_to_bottom(int i);
        g_cell();
        ~g_cell();
    private:
        unsigned int left_capacity, right_capacity, top_capacity, bottom_capacity;
        double horizontal_cost, vertical_cost;
        std::unordered_set<int> left_net_list, right_net_list, top_net_list, bottom_net_list;
};

#endif  // G_CELL_H
