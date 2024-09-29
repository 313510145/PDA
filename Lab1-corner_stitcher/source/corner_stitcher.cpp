#include "corner_stitcher.h"
#include <algorithm>

void corner_stitcher::input_information(std::istream& is) {
    is >> this->area.x >> this->area.y;
    tile* initial_space = new tile(SPACE_INDEX, coordinate(0, 0), coordinate(this->area.x, this->area.y));
    this->tile_list.push_back(initial_space);
    std::string temp;
    while (is >> temp) {
        if (temp == "P") {
            coordinate c;
            is >> c.x >> c.y;
            point_finding_to_buffer(c);
        }
        else {
            int index = std::stoi(temp);
            coordinate lower_left, a;
            is >> lower_left.x >> lower_left.y >> a.x >> a.y;
            block_creating(index, lower_left, a);
        }
    }
}

void corner_stitcher::point_finding_to_buffer(const coordinate& p) {
    const tile* st = this->tile_list[0];
    while (
        p.y < st->get_lower_left_y() ||
        p.y >= st->get_upper_right_y() ||
        p.x < st->get_lower_left_x() ||
        p.x >= st->get_upper_right_x()
    ) {
        while (p.y < st->get_lower_left_y()) {
            st = st->get_left_bottom();
        }
        while (p.y >= st->get_upper_right_y()) {
            st = st->get_right_top();
        }
        while (p.x < st->get_lower_left_x()) {
            st = st->get_bottom_left();
        }
        while (p.x >= st->get_upper_right_x()) {
            st = st->get_top_right();
        }
    }
    this->output_buffer.push_back(st->get_lower_left());
}

tile* corner_stitcher::point_finding(const coordinate& p) {
    tile* st = this->tile_list[0];
    while (
        p.y <= st->get_lower_left_y() ||
        p.y > st->get_upper_right_y() ||
        p.x < st->get_lower_left_x() ||
        p.x >= st->get_upper_right_x()
    ) {
        while (p.y <= st->get_lower_left_y()) {
            st = st->get_left_bottom();
        }
        while (p.y > st->get_upper_right_y()) {
            st = st->get_right_top();
        }
        while (p.x < st->get_lower_left_x()) {
            st = st->get_bottom_left();
        }
        while (p.x >= st->get_upper_right_x()) {
            st = st->get_top_right();
        }
    }
    return st;
}

void corner_stitcher::block_creating(const int& i, coordinate& ll, coordinate& a) {
    a.x += ll.x;
    a.y += ll.y;
    tile* new_block = new tile(i, ll, a);
    this->tile_list.push_back(new_block);
    ll.y = a.y;
    tile* original_space = point_finding(ll);
    int new_block_lower_left_y = new_block->get_lower_left_y();
    if (original_space->get_upper_right_y() > new_block->get_upper_right_y()) {
        horizontal_cut(original_space, new_block->get_upper_right_y());
    }
    while (original_space != nullptr && original_space->get_lower_left_y() > new_block_lower_left_y) {
        vertical_cut(original_space, new_block);
        original_space = new_block->get_left_bottom();
    }
    if (original_space != nullptr && original_space->get_lower_left_y() < new_block_lower_left_y) {
        horizontal_cut(original_space, new_block_lower_left_y);
        original_space = original_space->get_right_top();
    }
    if (original_space != nullptr) {
        vertical_cut(original_space, new_block);
    }
}

void corner_stitcher::horizontal_cut(tile* s, const int& nb_ur_y) {
    tile* upper_space = new tile(SPACE_INDEX, coordinate(s->get_lower_left_x(), nb_ur_y), coordinate(s->get_upper_right_x(), s->get_upper_right_y()));
    this->tile_list.push_back(upper_space);
    upper_space->set_left_bottom(s);
    upper_space->set_bottom_left(s->get_bottom_left());
    upper_space->set_right_top(s->get_right_top());
    upper_space->set_top_right(s->get_top_right());
    s->set_upper_right_y(nb_ur_y);
    s->set_right_top(upper_space);
    while (upper_space->get_bottom_left() != nullptr && upper_space->get_bottom_left()->get_upper_right_y() <= nb_ur_y) {
        upper_space->set_bottom_left(upper_space->get_bottom_left()->get_right_top());
    }
    while (s->get_top_right() != nullptr && s->get_top_right()->get_lower_left_y() >= nb_ur_y) {
        s->set_top_right(s->get_top_right()->get_left_bottom());
    }
    tile* tile_search = upper_space->get_bottom_left();
    while (tile_search != nullptr && tile_search->get_upper_right_y() <= upper_space->get_upper_right_y()) {
        tile_search->set_top_right(upper_space);
        tile_search = tile_search->get_right_top();
    }
    tile_search = upper_space->get_top_right();
    while (tile_search != nullptr && tile_search->get_lower_left_y() >= upper_space->get_lower_left_y()) {
        tile_search->set_bottom_left(upper_space);
        tile_search = tile_search->get_left_bottom();
    }
    tile_search = upper_space->get_right_top();
    while (tile_search != nullptr && tile_search->get_lower_left_x() >= upper_space->get_lower_left_x()) {
        tile_search->set_left_bottom(upper_space);
        tile_search = tile_search->get_bottom_left();
    }
}

void corner_stitcher::vertical_cut(tile* s, tile* b) {
    b->set_lower_left_y(s->get_lower_left_y());
    b->set_left_bottom(s->get_left_bottom());
    while (b->get_left_bottom() != nullptr && b->get_left_bottom()->get_upper_right_x() <= b->get_lower_left_x()) {
        b->set_left_bottom(b->get_left_bottom()->get_top_right());
    }
    tile* right_tile = b;
    if (b->get_upper_right_x() == s->get_upper_right_x()) {
        if (b->get_upper_right_y() == s->get_upper_right_y()) {
            b->set_top_right(s->get_top_right());
            b->set_right_top(s->get_right_top());
        }
    }
    else {
        right_tile = new tile(SPACE_INDEX, coordinate(b->get_upper_right_x(), s->get_lower_left_y()), coordinate(s->get_upper_right_x(), s->get_upper_right_y()));
        this->tile_list.push_back(right_tile);
        right_tile->set_left_bottom(b->get_left_bottom());
        right_tile->set_bottom_left(b);
        right_tile->set_right_top(s->get_right_top());
        right_tile->set_top_right(s->get_top_right());
        while (right_tile->get_left_bottom() != nullptr && right_tile->get_left_bottom()->get_upper_right_x() <= right_tile->get_lower_left_x()) {
            right_tile->set_left_bottom(right_tile->get_left_bottom()->get_top_right());
        }
        if (b->get_upper_right_y() == s->get_upper_right_y()) {
            b->set_top_right(right_tile);
            b->set_right_top(s->get_right_top());
            while (b->get_right_top() != nullptr && b->get_right_top()->get_lower_left_x() >= b->get_upper_right_x()) {
                b->set_right_top(b->get_right_top()->get_bottom_left());
            }
        }
    }
    tile* tile_search = s->get_top_right();
    while (tile_search != nullptr && tile_search->get_lower_left_y() >= s->get_lower_left_y()) {
        tile_search->set_bottom_left(right_tile);
        tile_search = tile_search->get_left_bottom();
    }
    tile_search = s->get_right_top();
    while (tile_search != nullptr && tile_search->get_lower_left_x() >= b->get_lower_left_x()) {
        if (tile_search != b) {
            if (tile_search->get_lower_left_x() >= b->get_upper_right_x()) {
                tile_search->set_left_bottom(right_tile);
            }
            else {
                tile_search->set_left_bottom(b);
            }
        }
        tile_search = tile_search->get_bottom_left();
    }
    tile_search = b->get_left_bottom();
    while (tile_search != nullptr && tile_search->get_upper_right_x() <= s->get_upper_right_x()) {
        if (tile_search->get_upper_right_x() > b->get_upper_right_x()) {
            tile_search->set_right_top(right_tile);
        }
        else {
            tile_search->set_right_top(b);
        }
        tile_search = tile_search->get_top_right();
    }
    if (b->get_lower_left_x() != s->get_lower_left_x()) {
        s->set_upper_right_x(b->get_lower_left_x());
        s->set_top_right(b);
        while (s->get_right_top() != nullptr && s->get_right_top()->get_lower_left_x() >= s->get_upper_right_x()) {
            s->set_right_top(s->get_right_top()->get_bottom_left());
        }
        b->set_bottom_left(s);
    }
    else {
        tile_search = s->get_bottom_left();
        b->set_bottom_left(s->get_bottom_left());
        while (tile_search != nullptr && tile_search->get_upper_right_y() <= s->get_upper_right_y()) {
            tile_search->set_top_right(b);
            tile_search = tile_search->get_right_top();
        }
        delete_space_in_list(s);
        s = b;
    }
    if (right_tile->get_left_bottom() != nullptr && right_tile != b) {
        merge(right_tile);
        merge(right_tile->get_left_bottom());
    }
    else if (right_tile != b) {
        merge(right_tile);
    }
    if (s->get_left_bottom() != nullptr && s != b) {
        merge(s);
        merge(s->get_left_bottom());
    }
    else if (s != b) {
        merge(s);
    }
}

void corner_stitcher::merge(tile* bs) {
    tile* top_space = bs->get_right_top();
    if (
        top_space != nullptr &&
        top_space->get_index() == SPACE_INDEX &&
        bs->get_index() == SPACE_INDEX &&
        bs->get_upper_right_x() == top_space->get_upper_right_x() &&
        bs->get_lower_left_x() == top_space->get_lower_left_x()
    ) {
        bs->set_upper_right_y(top_space->get_upper_right_y());
        bs->set_top_right(top_space->get_top_right());
        bs->set_right_top(top_space->get_right_top());
        tile* tile_search = bs->get_top_right();
        while (tile_search != nullptr && tile_search->get_lower_left_y() >= top_space->get_lower_left_y()) {
            tile_search->set_bottom_left(bs);
            tile_search = tile_search->get_left_bottom();
        }
        tile_search = bs->get_bottom_left();
        while (tile_search != nullptr && tile_search->get_upper_right_y() <= top_space->get_upper_right_y()) {
            tile_search->set_top_right(bs);
            tile_search = tile_search->get_right_top();
        }
        tile_search = bs->get_right_top();
        while (tile_search != nullptr && tile_search->get_lower_left_x() >= top_space->get_lower_left_x()) {
            tile_search->set_left_bottom(bs);
            tile_search = tile_search->get_bottom_left();
        }
        delete_space_in_list(top_space);
    }
}

void corner_stitcher::delete_space_in_list(tile* s) {
    this->tile_list.erase(find(this->tile_list.begin(), this->tile_list.end(), s));
    delete s;
}

void corner_stitcher::output_layout(std::ostream& os) const {
    os << this->tile_list.size() << std::endl
       << this->area.x << " " << this->area.y << std::endl;
    for (const auto* bl: this->tile_list) {
        os << bl->get_index() << " " << bl->get_lower_left_x() << " " << bl->get_lower_left_y() << " " << bl->get_upper_right_x() - bl->get_lower_left_x() << " " << bl->get_upper_right_y() - bl->get_lower_left_y() << std::endl;
    }
    os << std::endl;
}

bool tile_sorting_function(const tile* a, const tile* b) {
    return a->get_index() < b->get_index();
}

void corner_stitcher::output_tile_list_neighbor_num(std::ostream& os) {
    os << this->tile_list.size() << std::endl;
    int block_num, space_num;
    std::sort(this->tile_list.begin(), this->tile_list.end(), tile_sorting_function);
    for (const auto* bl: this->tile_list) {
        if (bl->get_index() > 0) {
            os << bl->get_index() << " ";
            neighbor_count(bl, block_num, space_num);
            os << block_num << " " << space_num << std::endl;
        }
    }
}

void corner_stitcher::output_point_finding(std::ostream& os) const {
    for (const auto& ob: this->output_buffer) {
        os << ob.x << " " << ob.y << std::endl;
    }
}

void corner_stitcher::neighbor_count(const tile* b, int& bn, int& sn) {
    bn = 0;
    sn = 0;
    const tile* tile_search = b->get_left_bottom();
    while (tile_search != nullptr && tile_search->get_lower_left_x() < b->get_upper_right_x()) {
        if (tile_search->get_index() == SPACE_INDEX) {
            sn++;
        }
        else {
            bn++;
        }
        tile_search = tile_search->get_top_right();
    }
    tile_search = b->get_bottom_left();
    while (tile_search != nullptr && tile_search->get_lower_left_y() < b->get_upper_right_y()) {
        if (tile_search->get_index() == SPACE_INDEX) {
            sn++;
        }
        else {
            bn++;
        }
        tile_search = tile_search->get_right_top();
    }
    tile_search = b->get_right_top();
    while (tile_search != nullptr && tile_search->get_upper_right_x() > b->get_lower_left_x()) {
        if (tile_search->get_index() == SPACE_INDEX) {
            sn++;
        }
        else {
            bn++;
        }
        tile_search = tile_search->get_bottom_left();
    }
    tile_search = b->get_top_right();
    while (tile_search != nullptr && tile_search->get_upper_right_y() > b->get_lower_left_y()) {
        if (tile_search->get_index() == SPACE_INDEX) {
            sn++;
        }
        else {
            bn++;
        }
        tile_search = tile_search->get_left_bottom();
    }
}

corner_stitcher::corner_stitcher() {
    this->area.x = 0;
    this->area.y = 0;
}

corner_stitcher::~corner_stitcher() {
    for (auto* bl: this->tile_list) {
        delete bl;
    }
    this->tile_list.clear();
    this->output_buffer.clear();
}
