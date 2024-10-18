#include "fixed_outline_floorplanner.h"
#include <math.h>
#include <algorithm>

void fixed_outline_floorplanner::input_block(std::istream& is) {
    std::string temp, terminal;
    int block_num, terminal_num;
    int x_temp, y_temp;
    block* b;
    is >> temp >> this->outline.x >> this->outline.y
       >> temp >> block_num
       >> temp >> terminal_num;
    for (auto i = 0; i < block_num; i++) {
        is >> temp >> x_temp >> y_temp;
        b = new block(false, temp, x_temp, y_temp);
        this->block_list.push_back(b);
    }
    this->sequence_pair_positive.resize(block_num);
    this->sequence_pair_negative.resize(block_num);
    for (auto i = 0; i < terminal_num; i++) {
        is >> temp >> terminal >> x_temp >> y_temp;
        b = new block(true, temp, x_temp, y_temp);
        this->block_list.push_back(b);
    }
}

void fixed_outline_floorplanner::input_net(std::istream& is) {
    std::string temp;
    int net_num, net_degree;
    is >> temp >> net_num;
    for (auto i = 0; i < net_num; i++) {
        is >> temp >> net_degree;
        net* n = new net();
        for (auto j = 0; j < net_degree; j++) {
            is >> temp;
            auto it = std::find_if(this->block_list.begin(), this->block_list.end(), [&temp](const block* k) {return temp == k->get_name();});
            if (it != this->block_list.end()) {
                n->add_block(*it);
            }
        }
        this->net_list.push_back(n);
    }
}

void fixed_outline_floorplanner::simulated_annealing() {
    double i;
    unsigned int a, b;
    unsigned int choice;
    clock_t start;
    start = clock();
    this->best_area_value = 0x7FFFFFFF;
    this->best_cost = 0x7FFFFFFF;
    do {
        initialize_sequence_pair();
        this->temperature = MAX_TEMPERATURE;
        do {
            a = static_cast<unsigned int>(static_cast<double>(this->sequence_pair_positive.size()) * rand() / (RAND_MAX + 1.0));
            do {
                b = static_cast<unsigned int>(static_cast<double>(this->sequence_pair_positive.size()) * rand() / (RAND_MAX + 1.0));
            } while (a == b);
            choice = static_cast<unsigned int>(static_cast<double>(OPERATION_NUM) * rand() / (RAND_MAX + 1.0));
            choice_and_index_operation(choice, a, b);
            place_in();
            calculate_extra_area();
            if (this->area_value < this->best_area_value) {
                update_sequence_pair();
                this->best_area_value = this->area_value;
            }
            else {
                i = rand() / (RAND_MAX + 1.0);
                if (i < exp((this->best_area_value - this->area_value) / this->temperature)) {
                    update_sequence_pair();
                    this->best_area_value = this->area_value;
                }
                else {
                    choice_and_index_operation(choice, a, b);
                }
            }
            this->temperature *= REDUCTION_RATE;
        } while (this->temperature > MIN_TEMPERATURE);
    } while (this->best_area_value > 0);
    retrieve_sequence_pair();
    this->temperature = MAX_TEMPERATURE;
    do {
        a = static_cast<unsigned int>(static_cast<double>(this->sequence_pair_positive.size()) * rand() / (RAND_MAX + 1.0));
        do {
            b = static_cast<unsigned int>(static_cast<double>(this->sequence_pair_positive.size()) * rand() / (RAND_MAX + 1.0));
        } while (a == b);
        choice = static_cast<unsigned int>(static_cast<double>(OPERATION_NUM) * rand() / (RAND_MAX + 1.0));
        choice_and_index_operation(choice, a, b);
        place_in();
        calculate_area();
        calculate_cost();
        if (this->area.x <= this->outline.x && this->area.y <= this->outline.y) {
            if (this->cost < this->best_cost) {
                update_sequence_pair();
                this->best_cost = this->cost;
            }
            else {
                i = rand() / (RAND_MAX + 1.0);
                if (i < exp((this->best_cost - this->cost) / this->temperature)) {
                    update_sequence_pair();
                    this->best_cost = this->cost;
                }
                else {
                    choice_and_index_operation(choice, a, b);
                }
            }
        }
        else {
            choice_and_index_operation(choice, a, b);
        }
        this->temperature *= REDUCTION_RATE;
    } while (this->temperature > MIN_TEMPERATURE);
    retrieve_sequence_pair();
    place_in();
    calculate_area();
    calculate_cost();
    this->run_time = (double)(clock() - start) / CLOCKS_PER_SEC;
}

void fixed_outline_floorplanner::output_report(std::ostream& os) const {
    os << this->cost << std::endl
       << this->hpwl << std::endl
       << this->area_value << std::endl
       << this->area.x << " " << this->area.y << std::endl
       << this->run_time << std::endl;
    for (const auto* i: this->sequence_pair_positive) {
        os << i->get_name() << " " << i->get_lower_left_x() << " " << i->get_lower_left_y() << " " << i->get_upper_right_x() << " " << i->get_upper_right_y() << std::endl;
    }
}

fixed_outline_floorplanner::fixed_outline_floorplanner() {
    this->temperature = 0;
    this->run_time = 0;
    this->alpha = 0;
    this->cost = 0;
    this->best_cost = 0;
    this->area_value = 0;
    this->best_area_value = 0;
    this->hpwl = 0;
}

fixed_outline_floorplanner::fixed_outline_floorplanner(char* alpha_in) {
    this->temperature = 0;
    this->run_time = 0;
    std::string ai(alpha_in);
    this->alpha = stod(ai);
    this->cost = 0;
    this->best_cost = 0;
    this->area_value = 0;
    this->best_area_value = 0;
    this->hpwl = 0;
}

fixed_outline_floorplanner::~fixed_outline_floorplanner() {
    for (auto i: this->net_list) {
        i->clear_block_list();
        delete i;
    }
    this->net_list.clear();
    for (auto i: this->block_list) {
        delete i;
    }
    this->block_list.clear();
    this->sequence_pair_positive.clear();
    this->best_sequence_pair_positive.clear();
    this->sequence_pair_negative.clear();
    this->best_sequence_pair_negative.clear();
}

void fixed_outline_floorplanner::initialize_sequence_pair() {
    srand(time(NULL));
    unsigned int temp;
    for (unsigned int i = 0; i < this->sequence_pair_positive.size();) {
        temp = static_cast<unsigned int>(static_cast<double>(this->sequence_pair_positive.size()) * rand() / (RAND_MAX + 1.0));
        if (this->block_list[temp]->get_flag() == false) {
            this->block_list[temp]->set_flag();
            this->sequence_pair_positive[i] = this->block_list[temp];
            i++;
        }
    }
    for (unsigned int i = 0; i < this->sequence_pair_negative.size();) {
        temp = static_cast<unsigned int>(static_cast<double>(this->sequence_pair_negative.size()) * rand() / (RAND_MAX + 1.0));
        if (this->block_list[temp]->get_flag() == true) {
            this->block_list[temp]->reset_flag();
            this->block_list[temp]->set_negative_index(i);
            this->sequence_pair_negative[i] = this->block_list[temp];
            i++;
        }
    }
}

void fixed_outline_floorplanner::swap_in_positive(unsigned int a, unsigned int b) {
    block* temp = this->sequence_pair_positive[a];
    this->sequence_pair_positive[a] = this->sequence_pair_positive[b];
    this->sequence_pair_positive[b] = temp;
}

void fixed_outline_floorplanner::swap_in_negative(unsigned int a, unsigned int b) {
    block* temp = this->sequence_pair_negative[a];
    this->sequence_pair_negative[a] = this->sequence_pair_negative[b];
    this->sequence_pair_negative[b] = temp;
    unsigned int temp_index = this->sequence_pair_negative[a]->get_negative_index();
    this->sequence_pair_negative[a]->set_negative_index(this->sequence_pair_negative[b]->get_negative_index());
    this->sequence_pair_negative[b]->set_negative_index(temp_index);
}

void fixed_outline_floorplanner::swap_in_both(unsigned int a, unsigned int b) {
    block* temp = this->sequence_pair_positive[a];
    this->sequence_pair_positive[a] = this->sequence_pair_positive[b];
    this->sequence_pair_positive[b] = temp;
    a = this->sequence_pair_positive[a]->get_negative_index();
    b = this->sequence_pair_positive[b]->get_negative_index();
    temp = this->sequence_pair_negative[a];
    this->sequence_pair_negative[a] = this->sequence_pair_negative[b];
    this->sequence_pair_negative[b] = temp;
    unsigned int temp_index = this->sequence_pair_negative[a]->get_negative_index();
    this->sequence_pair_negative[a]->set_negative_index(this->sequence_pair_negative[b]->get_negative_index());
    this->sequence_pair_negative[b]->set_negative_index(temp_index);
}

void fixed_outline_floorplanner::rotate(unsigned int i) {
    int temp_a = this->sequence_pair_positive[i]->get_upper_right_x() - this->sequence_pair_positive[i]->get_lower_left_x();
    int temp_b = this->sequence_pair_positive[i]->get_upper_right_y() - this->sequence_pair_positive[i]->get_lower_left_y();
    this->sequence_pair_positive[i]->set_upper_right_x(this->sequence_pair_positive[i]->get_lower_left_x() + temp_b);
    this->sequence_pair_positive[i]->set_upper_right_y(this->sequence_pair_positive[i]->get_lower_left_y() + temp_a);
}

void fixed_outline_floorplanner::place_in() {
    for (auto i: this->sequence_pair_positive) {
        i->set_upper_right_x(i->get_upper_right_x() - i->get_lower_left_x());
        i->set_upper_right_y(i->get_upper_right_y() - i->get_lower_left_y());
        i->set_lower_left_x(0);
        i->set_lower_left_y(0);
    }
    for (unsigned int i = 0; i < this->sequence_pair_positive.size() - 1; i++) {
        for (unsigned int j = i + 1; j < this->sequence_pair_negative.size(); j++) {
            unsigned int i_negative = this->sequence_pair_positive[i]->get_negative_index();
            unsigned int j_negative = this->sequence_pair_positive[j]->get_negative_index();
            if (j_negative > i_negative && this->sequence_pair_positive[i]->get_upper_right_x() > this->sequence_pair_positive[j]->get_lower_left_x())  {
                this->sequence_pair_positive[j]->set_upper_right_x(this->sequence_pair_positive[j]->get_upper_right_x() - this->sequence_pair_positive[j]->get_lower_left_x() + this->sequence_pair_positive[i]->get_upper_right_x());
                this->sequence_pair_positive[j]->set_lower_left_x(this->sequence_pair_positive[i]->get_upper_right_x());
            }
            else if (j_negative < i_negative && this->sequence_pair_positive[i]->get_upper_right_y() > this->sequence_pair_positive[j]->get_lower_left_y()) {
                this->sequence_pair_positive[j]->set_upper_right_y(this->sequence_pair_positive[j]->get_upper_right_y() - this->sequence_pair_positive[j]->get_lower_left_y() + this->sequence_pair_positive[i]->get_upper_right_y());
                this->sequence_pair_positive[j]->set_lower_left_y(this->sequence_pair_positive[i]->get_upper_right_y());
            }
        }
    }
}

void fixed_outline_floorplanner::calculate_area() {
    this->area.x = 0;
    this->area.y = 0;
    for (unsigned int i = 0; i < this->sequence_pair_positive.size(); i++) {
        if (this->block_list[i]->get_upper_right_y() > this->area.y) {
            this->area.y = this->block_list[i]->get_upper_right_y();
        }
        if (this->block_list[i]->get_upper_right_x() > this->area.x) {
            this->area.x = this->block_list[i]->get_upper_right_x();
        }
    }
    this->area_value = this->area.x * this->area.y;
}

void fixed_outline_floorplanner::calculate_extra_area() {
    calculate_area();
    if (this->area.x > this->outline.x && this->area.y > this->outline.y) {
        this->area_value -= this->outline.x * this->outline.y;
    }
    else if (this->area.x > this->outline.x) {
        this->area_value -= this->outline.x * this->area.y;
    }
    else if (this->area.y > this->outline.y) {
        this->area_value -= this->area.x * this->outline.y;
    }
    else {
        this->area_value -= this->outline.x * this->outline.y;
    }
}

void fixed_outline_floorplanner::calculate_cost() {
    coordinate ll, ur;
    this->hpwl = 0;
    for (const auto* i: this->net_list) {
        ur.x = 0;
        ur.y = 0;
        ll.x = 0x7FFFFFFF;
        ll.y = 0x7FFFFFFF;
        for (unsigned int j = 0; j < i->get_block_list_size(); j++) {
            if (i->get_block(j)->get_middle_x() > ur.x) {
                ur.x = i->get_block(j)->get_middle_x();
            }
            if (i->get_block(j)->get_middle_x() < ll.x) {
                ll.x = i->get_block(j)->get_middle_x();
            }
            if (i->get_block(j)->get_middle_y() > ur.y) {
                ur.y = i->get_block(j)->get_middle_y();
            }
            if (i->get_block(j)->get_middle_y() < ll.y) {
                ll.y = i->get_block(j)->get_middle_y();
            }
        }
        this->hpwl += (ur.x - ll.x + ur.y - ll.y);
    }
    this->cost = this->alpha * this->area_value + (1 - this->alpha) * this->hpwl;
}

void fixed_outline_floorplanner::update_sequence_pair() {
    this->best_sequence_pair_positive = this->sequence_pair_positive;
    this->best_sequence_pair_negative = this->sequence_pair_negative;
}

void fixed_outline_floorplanner::retrieve_sequence_pair() {
    this->sequence_pair_positive = this->best_sequence_pair_positive;
    this->sequence_pair_negative = this->best_sequence_pair_negative;
}

void fixed_outline_floorplanner::choice_and_index_operation(unsigned int choice, unsigned int a, unsigned int b) {
    switch (choice) {
        case SWAP_IN_POSITIVE:
            swap_in_positive(a, b);
            break;
        case SWAP_IN_NEGATIVE:
            swap_in_negative(a, b);
            break;
        case SWAP_IN_BOTH:
            swap_in_both(a, b);
            break;
        case ROTATE:
            rotate(a);
            break;
        default:
            break;
    }
}
