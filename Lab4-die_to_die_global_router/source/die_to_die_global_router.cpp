#include "die_to_die_global_router.h"
#include <string>
#include <cfloat>
#include <queue>
#include <stack>

void die_to_die_global_router::input_map(std::istream& is) {
    std::string temp_str;
    xy xy_a, xy_b;
    is >> temp_str >> this->lower_left_x >> this->lower_left_y
       >> this->grid_x_num >> this->grid_y_num;
    is >> temp_str >> this->grid_width >> this->grid_height;
    this->grid_x_num /= this->grid_width;
    this->grid_y_num /= this->grid_height;
    for (int i = 0; i < grid_y_num; i++) {
        std::vector<g_cell> v;
        for (int j = 0; j < grid_x_num; j++) {
            v.push_back(g_cell());
        }
        this->grid_map.push_back(v);
        v.clear();
    }
    is >> temp_str >> xy_a.x >> xy_a.y >> temp_str >> temp_str
       >> temp_str
       >> temp_str;
    while (temp_str != ".c") {
        is >> xy_b.x >> xy_b.y;
        xy_b.x = (xy_a.x + xy_b.x) / this->grid_width;
        xy_b.y = (xy_a.y + xy_b.y) / this->grid_height;
        this->net_list[stoi(temp_str)] = std::make_pair(xy_b, xy_b);
        is >> temp_str;
    }
    is >> xy_a.x >> xy_a.y >> temp_str >> temp_str
       >> temp_str;
    while (is >> temp_str) {
        is >> xy_b.x >> xy_b.y;
        xy_b.x = (xy_a.x + xy_b.x) / this->grid_width;
        xy_b.y = (xy_a.y + xy_b.y) / this->grid_height;
        this->net_list[stoi(temp_str)].second = xy_b;
    }
}

void die_to_die_global_router::input_g_cell(std::istream& is) {
    std::string temp_str;
    unsigned int temp;
    is >> temp_str;
    for (int i = 0; i < this->grid_y_num; i++) {
        for (int j = 0; j < this->grid_x_num; j++) {
            is >> temp;
            this->grid_map[i][j].set_left_capacity(temp);
            if (j != 0) {
                this->grid_map[i][j - 1].set_right_capacity(temp);
            }
            is >> temp;
            this->grid_map[i][j].set_bottom_capacity(temp);
            if (i != 0) {
                this->grid_map[i - 1][j].set_top_capacity(temp);
            }
        }
    }
}

void die_to_die_global_router::input_cost(std::istream& is) {
    std::string temp_str;
    double temp;
    is >> temp_str >> this->alpha
       >> temp_str >> this->beta
       >> temp_str >> this->gamma
       >> temp_str >> this->delta
       >> temp_str >> this->via_cost;
    is >> temp_str;
    for (int i = 0; i < this->grid_y_num; i++) {
        for (int j = 0; j < this->grid_x_num; j++) {
            is >> temp;
            this->grid_map[i][j].set_vertical_cost(temp);
            if (temp > this->max_vertical_cell_cost) {
                this->max_vertical_cell_cost = temp;
            }
        }
    }
    is >> temp_str;
    for (int i = 0; i < this->grid_y_num; i++) {
        for (int j = 0; j < this->grid_x_num; j++) {
            is >> temp;
            this->grid_map[i][j].set_horizontal_cost(temp);
            if (temp > this->max_horizontal_cell_cost) {
                this->max_horizontal_cell_cost = temp;
            }
        }
    }
}

void die_to_die_global_router::a_star_search() {
    xy next_movement[DIRECTION_NUM];
    next_movement[LEFT].x = -1;
    next_movement[RIGHT].x = 1;
    next_movement[UP].y = 1;
    next_movement[DOWN].y = -1;
    bool** traversed_map = new bool*[this->grid_y_num];
    double** f_map = new double*[this->grid_y_num];
    int** parent_map = new int*[this->grid_y_num];
    for (int i = 0; i < this->grid_y_num; i++) {
        traversed_map[i] = new bool[this->grid_x_num];
        f_map[i] = new double[this->grid_x_num];
        parent_map[i] = new int[this->grid_x_num];
    }
    for (auto nl: this->net_list) {
        node net_start(nl.second.first.x, nl.second.first.y);
        node net_end(nl.second.second.x, nl.second.second.y);
        for (int i = 0; i < this->grid_y_num; i++) {
            for (int j = 0; j < this->grid_x_num; j++) {
                traversed_map[i][j] = 0;
                f_map[i][j] = 0;
                parent_map[i][j] = -1;
            }
        }
        net_start.f = this->alpha * (this->grid_width * abs(net_end.coordinate.x - net_start.coordinate.x) + this->grid_height * abs(net_end.coordinate.y - net_start.coordinate.y));
        std::priority_queue<node> pq;
        pq.push(net_start);
        while (!pq.empty()) {
            node cell_current = pq.top();
            pq.pop();
            traversed_map[cell_current.coordinate.y][cell_current.coordinate.x] = true;
            if (cell_current.coordinate.x == net_end.coordinate.x && cell_current.coordinate.y == net_end.coordinate.y) {
                break;
            }
            for (int i = 0; i < DIRECTION_NUM; i++) {
                node cell_next(cell_current.coordinate.x + next_movement[i].x, cell_current.coordinate.y + next_movement[i].y);
                if (
                    cell_next.coordinate.x >= 0 && cell_next.coordinate.x < this->grid_x_num &&
                    cell_next.coordinate.y >= 0 && cell_next.coordinate.y < this->grid_y_num &&
                    !traversed_map[cell_next.coordinate.y][cell_next.coordinate.x]
                ) {
                    int cell_current_parent_direction = parent_map[cell_current.coordinate.y][cell_current.coordinate.x];
                    cell_next.g = cell_current.g;
                    switch (i) {
                        case LEFT:
                            cell_next.g += 0.5 * this->alpha * this->grid_width;
                            cell_next.g += 0.5 * this->beta * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_left_overflow() * this->max_horizontal_cell_cost;
                            cell_next.g += 0.5 * this->gamma * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_horizontal_cost();
                            switch (cell_current_parent_direction) {
                                case RIGHT:
                                    cell_next.g += 0.5 * this->alpha * this->grid_width;
                                    cell_next.g += 0.5 * this->beta * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_right_overflow() * this->max_horizontal_cell_cost;
                                    cell_next.g += 0.5 * this->gamma * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_horizontal_cost();
                                    break;
                                case UP:
                                    cell_next.g += 0.5 * this->alpha * this->grid_height;
                                    cell_next.g += 0.5 * this->beta * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_top_overflow() * this->max_vertical_cell_cost;
                                    cell_next.g += 0.5 * this->gamma * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_vertical_cost();
                                    cell_next.g += this->delta * this->via_cost;
                                    break;
                                case DOWN:
                                    cell_next.g += 0.5 * this->alpha * this->grid_height;
                                    cell_next.g += 0.5 * this->beta * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_bottom_overflow() * this->max_vertical_cell_cost;
                                    cell_next.g += 0.5 * this->gamma * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_vertical_cost();
                                    cell_next.g += this->delta * this->via_cost;
                                    break;
                                default:
                                    cell_next.g += this->delta * this->via_cost;
                                    break;
                            }
                            break;
                        case RIGHT:
                            cell_next.g += 0.5 * this->alpha * this->grid_width;
                            cell_next.g += 0.5 * this->beta * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_right_overflow() * this->max_horizontal_cell_cost;
                            cell_next.g += 0.5 * this->gamma * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_horizontal_cost();
                            switch (cell_current_parent_direction) {
                                case LEFT:
                                    cell_next.g += 0.5 * this->alpha * this->grid_width;
                                    cell_next.g += 0.5 * this->beta * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_left_overflow() * this->max_horizontal_cell_cost;
                                    cell_next.g += 0.5 * this->gamma * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_horizontal_cost();
                                    break;
                                case UP:
                                    cell_next.g += 0.5 * this->alpha * this->grid_height;
                                    cell_next.g += 0.5 * this->beta * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_top_overflow() * this->max_vertical_cell_cost;
                                    cell_next.g += 0.5 * this->gamma * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_vertical_cost();
                                    cell_next.g += this->delta * this->via_cost;
                                    break;
                                case DOWN:
                                    cell_next.g += 0.5 * this->alpha * this->grid_height;
                                    cell_next.g += 0.5 * this->beta * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_bottom_overflow() * this->max_vertical_cell_cost;
                                    cell_next.g += 0.5 * this->gamma * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_vertical_cost();
                                    cell_next.g += this->delta * this->via_cost;
                                    break;
                                default:
                                    cell_next.g += this->delta * this->via_cost;
                                    break;
                            }
                            break;
                        case UP:
                            cell_next.g += 0.5 * this->alpha * this->grid_height;
                            cell_next.g += 0.5 * this->beta * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_top_overflow() * this->max_vertical_cell_cost;
                            cell_next.g += 0.5 * this->gamma * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_vertical_cost();
                            switch (cell_current_parent_direction) {
                                case LEFT:
                                    cell_next.g += 0.5 * this->alpha * this->grid_width;
                                    cell_next.g += 0.5 * this->beta * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_left_overflow() * this->max_horizontal_cell_cost;
                                    cell_next.g += 0.5 * this->gamma * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_horizontal_cost();
                                    cell_next.g += this->delta * this->via_cost;
                                    break;
                                case RIGHT:
                                    cell_next.g += 0.5 * this->alpha * this->grid_width;
                                    cell_next.g += 0.5 * this->beta * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_right_overflow() * this->max_horizontal_cell_cost;
                                    cell_next.g += 0.5 * this->gamma * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_horizontal_cost();
                                    cell_next.g += this->delta * this->via_cost;
                                    break;
                                case DOWN:
                                    cell_next.g += 0.5 * this->alpha * this->grid_height;
                                    cell_next.g += 0.5 * this->beta * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_bottom_overflow() * this->max_vertical_cell_cost;
                                    cell_next.g += 0.5 * this->gamma * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_vertical_cost();
                                    break;
                            }
                            break;
                        case DOWN:
                            cell_next.g += 0.5 * this->alpha * this->grid_height;
                            cell_next.g += 0.5 * this->beta * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_bottom_overflow() * this->max_vertical_cell_cost;
                            cell_next.g += 0.5 * this->gamma * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_vertical_cost();
                            switch (cell_current_parent_direction) {
                                case LEFT:
                                    cell_next.g += 0.5 * this->alpha * this->grid_width;
                                    cell_next.g += 0.5 * this->beta * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_left_overflow() * this->max_horizontal_cell_cost;
                                    cell_next.g += 0.5 * this->gamma * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_horizontal_cost();
                                    cell_next.g += this->delta * this->via_cost;
                                    break;
                                case RIGHT:
                                    cell_next.g += 0.5 * this->alpha * this->grid_width;
                                    cell_next.g += 0.5 * this->beta * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_right_overflow() * this->max_horizontal_cell_cost;
                                    cell_next.g += 0.5 * this->gamma * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_horizontal_cost();
                                    cell_next.g += this->delta * this->via_cost;
                                    break;
                                case UP:
                                    cell_next.g += 0.5 * this->alpha * this->grid_height;
                                    cell_next.g += 0.5 * this->beta * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_top_overflow() * this->max_vertical_cell_cost;
                                    cell_next.g += 0.5 * this->gamma * this->grid_map[cell_current.coordinate.y][cell_current.coordinate.x].get_vertical_cost();
                                    break;
                            }
                            break;
                    }
                    cell_next.f = cell_next.g + this->alpha * (this->grid_width * abs(net_end.coordinate.x - cell_next.coordinate.x) + this->grid_height * abs(net_end.coordinate.y - cell_next.coordinate.y));
                    if (f_map[cell_next.coordinate.y][cell_next.coordinate.x] == 0 || cell_next.f < f_map[cell_next.coordinate.y][cell_next.coordinate.x]) {
                        f_map[cell_next.coordinate.y][cell_next.coordinate.x] = cell_next.f;
                        parent_map[cell_next.coordinate.y][cell_next.coordinate.x] = REVERSE_DIRECTION[i];
                        pq.push(cell_next);
                    }
                }
            }
        }
        std::stack<xy> s;
        while (net_end.coordinate.x != net_start.coordinate.x || net_end.coordinate.y != net_start.coordinate.y) {
            s.push(net_end.coordinate);
            xy next_move = next_movement[parent_map[net_end.coordinate.y][net_end.coordinate.x]];
            net_end.coordinate.x += next_move.x;
            net_end.coordinate.y += next_move.y;
        }
        bool vertical = true;
        while (!s.empty()) {
            xy temp = s.top();
            s.pop();
            switch (parent_map[temp.y][temp.x]) {
                case LEFT:
                    this->grid_map[net_start.coordinate.y][net_start.coordinate.x].add_net_to_right(nl.first);
                    this->grid_map[temp.y][temp.x].add_net_to_left(nl.first);
                    if (vertical) {
                        this->net_list_path[nl.first].push_back(net_start.coordinate);
                        vertical = false;
                    }
                    break;
                case RIGHT:
                    this->grid_map[net_start.coordinate.y][net_start.coordinate.x].add_net_to_left(nl.first);
                    this->grid_map[temp.y][temp.x].add_net_to_right(nl.first);
                    if (vertical) {
                        this->net_list_path[nl.first].push_back(net_start.coordinate);
                        vertical = false;
                    }
                    break;
                case UP:
                    this->grid_map[net_start.coordinate.y][net_start.coordinate.x].add_net_to_bottom(nl.first);
                    this->grid_map[temp.y][temp.x].add_net_to_top(nl.first);
                    if (!vertical) {
                        this->net_list_path[nl.first].push_back(net_start.coordinate);
                        vertical = true;
                    }
                    break;
                case DOWN:
                    this->grid_map[net_start.coordinate.y][net_start.coordinate.x].add_net_to_top(nl.first);
                    this->grid_map[temp.y][temp.x].add_net_to_bottom(nl.first);
                    if (!vertical) {
                        this->net_list_path[nl.first].push_back(net_start.coordinate);
                        vertical = true;
                    }
                    break;
            }
            net_start.coordinate = temp;
        }
        this->net_list_path[nl.first].push_back(net_start.coordinate);
    }
    for (int i = 0; i < this->grid_y_num; i++) {
        delete [] traversed_map[i];
        delete [] f_map[i];
        delete [] parent_map[i];
    }
    delete [] traversed_map;
    delete [] f_map;
    delete [] parent_map;
}

void die_to_die_global_router::output_results(std::ostream& os) {
    double a, b, c;
    xy temp;
    for (auto nl: this->net_list) {
        os << "n" << nl.first << "\n";
        temp = nl.second.first;
        for (auto nlp: this->net_list_path[nl.first]) {
            if (nlp.x == temp.x) {
                if (nlp.y == temp.y) {
                    continue;
                }
                a = this->lower_left_x + this->grid_width * temp.x;
                b = this->lower_left_y + this->grid_height * temp.y;
                c = this->lower_left_y + this->grid_height * nlp.y;
                os << "M1 " << a << " " << b << " " << a << " " << c << "\n";
            }
            else {
                a = this->lower_left_x + this->grid_width * temp.x;
                b = this->lower_left_x + this->grid_width * nlp.x;
                c = this->lower_left_y + this->grid_height * temp.y;
                os << "via\n"
                    << "M2 " << a << " " << c << " " << b << " " << c << "\n"
                    << "via\n";
            }
            temp = nlp;
        }
        os << ".end\n";
    }
}

die_to_die_global_router::die_to_die_global_router() {
    this->lower_left_x = 0;
    this->lower_left_y = 0;
    this->grid_x_num = 0;
    this->grid_y_num = 0;
    this->grid_width = 0;
    this->grid_height = 0;
    this->alpha = 0;
    this->beta = 0;
    this->gamma = 0;
    this->delta = 0;
    this->via_cost = 0;
    this->max_horizontal_cell_cost = DBL_MIN;
    this->max_vertical_cell_cost = DBL_MIN;
}

die_to_die_global_router::~die_to_die_global_router() {
    for (auto gm: this->grid_map) {
        gm.clear();
    }
    this->grid_map.clear();
    this->net_list.clear();
    for (auto nlp: this->net_list_path) {
        nlp.second.clear();
    }
    this->net_list_path.clear();
}
