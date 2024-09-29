#ifndef TILE_H
#define TILE_H

struct coordinate {
    int x, y;
    coordinate() {
        this->x = 0;
        this->y = 0;
    }
    coordinate(int x_in, int y_in) {
        this->x = x_in;
        this->y = y_in;
    }
};

class tile {
    public:
        int get_index() const;
        int get_lower_left_x() const;
        int get_lower_left_y() const;
        coordinate get_lower_left() const;
        int get_upper_right_x() const;
        int get_upper_right_y() const;
        coordinate get_upper_right() const;
        void set_lower_left_x(int llx);
        void set_lower_left_y(int lly);
        void set_upper_right_x(int urx);
        void set_upper_right_y(int ury);
        tile* get_left_bottom() const;
        tile* get_bottom_left() const;
        tile* get_right_top() const;
        tile* get_top_right() const;
        void set_left_bottom(tile* lb);
        void set_bottom_left(tile* bl);
        void set_right_top(tile* rt);
        void set_top_right(tile* tr);
        tile();
        tile(int index_in, coordinate lower_left_in, coordinate upper_right_in);
        ~tile();
    private:
        int index;
        coordinate lower_left, upper_right;
        tile *left_bottom, *bottom_left, *right_top, *top_right;
};

#endif  // TILE_H
