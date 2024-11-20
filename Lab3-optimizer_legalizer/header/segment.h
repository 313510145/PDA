#ifndef SEGMENT_H
#define SEGMENT_H

class segment {
    public:
        double get_left_x() const;
        double get_right_x() const;
        double get_width() const;
        void set_left_x(double left_x_in);
        void set_right_x(double right_x_in);
        void set_width(double width_in);
        segment();
        ~segment();
    private:
        double left_x, right_x;
        double width;
};

#endif  // SEGMENT_H
