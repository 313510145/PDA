#ifndef BLOCK_H
#define BLOCK_H

#include <string>

class block {
    public:
        std::string get_name() const;
        double get_lower_left_x() const;
        double get_lower_left_y() const;
        double get_upper_right_x() const;
        double get_upper_right_y() const;
        double get_width() const;
        double get_height() const;
        bool get_fixed() const;
        void set_name(const std::string& name_in);
        void set_lower_left_x(double lower_left_x_in);
        void set_lower_left_y(double lower_left_y_in);
        void set_upper_right_x(double upper_right_x_in);
        void set_upper_right_y(double upper_right_y_in);
        void set_fixed(bool fixed_in);
        block();
        ~block();
    private:
        std::string name;
        double lower_left_x, lower_left_y;
        double upper_right_x, upper_right_y;
        bool fixed;
};

#endif  // BLOCK_H
