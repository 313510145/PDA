#ifndef BLOCK_H
#define BLOCK_H

#include <string>

struct coordinate {
    int x, y;
    coordinate() {
        this->x = 0;
        this->y = 0;
    }
};

class block {
    public:
        bool get_flag() const;
        std::string get_name() const;
        int get_lower_left_x() const;
        int get_lower_left_y() const;
        int get_upper_right_x() const;
        int get_upper_right_y() const;
        int get_middle_x() const;
        int get_middle_y() const;
        unsigned int get_negative_index() const;
        void set_flag();
        void reset_flag();
        void set_lower_left_x(int x_in);
        void set_lower_left_y(int y_in);
        void set_upper_right_x(int x_in);
        void set_upper_right_y(int y_in);
        void set_negative_index(unsigned int negative_index_in);
        block();
        block(bool is_terminal_in, const std::string& name_in, int x_in, int y_in);
        ~block();
    private:
        bool flag;
        std::string name;
        coordinate lower_left, upper_right;
        unsigned int negative_index;
};

#endif  // BLOCK_H
