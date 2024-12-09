#ifndef G_CELL_H
#define G_CELL_H

#include <unordered_set>

class g_cell {
    public:
        int get_horizontal_capacity() const;
        int get_vertical_capacity() const;
        double get_horizontal_cost() const;
        double get_vertical_cost() const;
        int get_left_overflow() const;
        int get_right_overflow() const;
        int get_top_overflow() const;
        int get_bottom_overflow() const;
        void set_horizontal_capacity(unsigned int horizontal_capacity_in);
        void set_vertical_capacity(unsigned int vertical_capacity_in);
        void set_horizontal_cost(double horizontal_cost_in);
        void set_vertical_cost(double vertical_cost_in);
        void add_net_to_left(int i);
        void add_net_to_right(int i);
        void add_net_to_top(int i);
        void add_net_to_bottom(int i);
        g_cell();
        ~g_cell();
    private:
        unsigned int horizontal_capacity, vertical_capacity;
        double horizontal_cost, vertical_cost;
        std::unordered_set<int> left_net_list, right_net_list, top_net_list, bottom_net_list;
};

#endif  // G_CELL_H
