#include "optimizer_legalizer.h"
#include <algorithm>
#include <cmath>
#include <float.h>

void optimizer_legalizer::input_site(std::istream& is) {
    std::vector<block*> block_list;
    std::string temp;
    double temp_double;
    is >> temp >> this->alpha >> temp >> this->beta >> temp
       >> this->lower_left_x >> this->lower_left_y
       >> this->upper_right_x >> this->upper_right_y;
    while (is >> temp) {
        if (temp != "PlacementRows") {
            block* b = new block();
            b->set_name(temp);
            is >> temp_double;
            b->set_lower_left_x(temp_double);
            is >> temp_double;
            b->set_lower_left_y(temp_double);
            is >> temp_double;
            b->set_upper_right_x(b->get_lower_left_x() + temp_double);
            is >> temp_double;
            b->set_upper_right_y(b->get_lower_left_y() + temp_double);
            is >> temp;
            if (temp == "FIX") {
                b->set_fixed(true);
            }
            else {
                b->set_fixed(false);
                this->block_name_map[b->get_name()] = b;
            }
            block_list.push_back(b);
        }
        else {
            segment* s = new segment();
            is >> temp_double;
            s->set_left_x(temp_double);
            is >> temp_double;
            this->segment_map[temp_double].push_back(s);
            is >> temp_double;
            s->set_width(temp_double);
            is >> temp_double >> temp_double;
            s->set_right_x(s->get_left_x() + temp_double * s->get_width());
        }
    }
    for (auto cl: block_list) {
        find_segments_and_cut(cl);
        if (cl->get_fixed()) {
            delete cl;
        }
    }
    block_list.clear();
}

void optimizer_legalizer::input_merged_block_and_output_result(std::istream& is, std::ostream& os) {
    std::string temp;
    double temp_double_a, temp_double_b;
    std::map<double, std::vector<segment*>>::iterator it, it_bottom;
    std::vector<std::vector<segment*>> block_segments;
    std::vector<segment*> segment_candidate;
    double best_x = 0;
    double best_y = 0;
    bool can_place;
    while (is >> temp) {
        is >> temp;
        while (temp != "-->") {
            if (this->block_name_map.find(temp) != this->block_name_map.end()) {
                block* ffl = this->block_name_map[temp];
                create_segments_and_merge(ffl);
                this->block_name_map.erase(ffl->get_name());
                delete ffl;
            }
            is >> temp;
        }
        block* b = new block();
        is >> temp;
        b->set_name(temp);
        is >> temp_double_a;
        b->set_lower_left_x(temp_double_a);
        is >> temp_double_a;
        b->set_lower_left_y(temp_double_a);
        is >> temp_double_a;
        b->set_upper_right_x(b->get_lower_left_x() + temp_double_a);
        is >> temp_double_a;
        b->set_upper_right_y(b->get_lower_left_y() + temp_double_a);
        it = this->segment_map.begin();
        it_bottom = it;
        while ((it != this->segment_map.end()) && ((it->first - it_bottom->first) < b->get_height())) {
            for (std::vector<segment*>::iterator s = it->second.begin(); s != it->second.end(); ++s) {
                if ((*s)->get_right_x() - (*s)->get_left_x() >= b->get_width()) {
                    segment_candidate.push_back(*s);
                }
            }
            block_segments.push_back(segment_candidate);
            segment_candidate.clear();
            it = this->segment_map.upper_bound(it->first);
        }
        double best_distance = DBL_MAX;
        while (it != this->segment_map.end()) {
            for (const auto bottom: block_segments[0]) {
                temp_double_a = bottom->get_left_x();
                while ((temp_double_b = (temp_double_a + b->get_width())) <= bottom->get_right_x()) {
                    can_place = true;
                    for (unsigned int i = 1; i < block_segments.size(); i++) {
                        auto above_segment = std::find_if(
                            block_segments[i].begin(),
                            block_segments[i].end(),
                            [&temp_double_a, &temp_double_b](const segment* s) {
                                return (s->get_left_x() <= temp_double_a && s->get_right_x() >= temp_double_b);
                            }
                        );
                        if (above_segment == block_segments[i].end()) {
                            can_place = false;
                            auto first_segment = std::find_if(
                                block_segments[i].begin(),
                                block_segments[i].end(),
                                [&temp_double_a](const segment* s) {
                                    return s->get_left_x() > temp_double_a;
                                }
                            );
                            if (first_segment != block_segments[i].end()) {
                                temp_double_a = (*first_segment)->get_left_x();
                            }
                            else {
                                temp_double_a = bottom->get_right_x();
                            }
                            break;
                        }
                    }
                    if (can_place) {
                        double temp_distance = fabs(it_bottom->first - b->get_lower_left_y()) + fabs(temp_double_a - b->get_lower_left_x());
                        if (temp_distance < best_distance) {
                            best_distance = temp_distance;
                            best_x = temp_double_a;
                            best_y = it_bottom->first;
                        }
                        temp_double_a += ceil(b->get_width() / bottom->get_width()) * bottom->get_width();
                    }
                }
            }
            it_bottom = this->segment_map.upper_bound(it_bottom->first);
            while ((it != this->segment_map.end()) && ((it->first - it_bottom->first) < b->get_height())) {
                for (std::vector<segment*>::iterator s = it->second.begin(); s != it->second.end(); ++s) {
                    if ((*s)->get_right_x() - (*s)->get_left_x() >= b->get_width()) {
                        segment_candidate.push_back(*s);
                    }
                }
                block_segments.push_back(segment_candidate);
                segment_candidate.clear();
                it = this->segment_map.upper_bound(it->first);
            }
            block_segments.erase(block_segments.begin());
        }
        temp_double_a = b->get_width();
        temp_double_b = b->get_height();
        b->set_lower_left_x(best_x);
        b->set_lower_left_y(best_y);
        b->set_upper_right_x(b->get_lower_left_x() + temp_double_a);
        b->set_upper_right_y(b->get_lower_left_y() + temp_double_b);
        this->block_name_map[b->get_name()] = b;
        os << b->get_lower_left_x() << " " << b->get_lower_left_y() << "\n"
           << "0\n";
        find_segments_and_cut(b);
        block_segments.clear();
    }
}

optimizer_legalizer::optimizer_legalizer() {
    this->alpha = 0;
    this->beta = 0;
    this->lower_left_x = 0;
    this->lower_left_y = 0;
    this->upper_right_x = 0;
    this->upper_right_y = 0;
}

optimizer_legalizer::~optimizer_legalizer() {
    for (auto bnm: this->block_name_map) {
        delete bnm.second;
    }
    this->block_name_map.clear();
    for (auto sm: this->segment_map) {
        for (auto s: sm.second) {
            delete s;
        }
        sm.second.clear();
    }
    this->segment_map.clear();
}

void optimizer_legalizer::find_segments_and_cut(block* b_in) {
    std::map<double, std::vector<segment*>>::iterator it = this->segment_map.lower_bound(b_in->get_lower_left_y());
    if (it->first != b_in->get_lower_left_y()) {
        --it;
    }
    do {
        auto it_segment = std::find_if(
            it->second.begin(),
            it->second.end(),
            [&b_in](const segment* s) {
                return (
                    b_in->get_lower_left_x() >= s->get_left_x() &&
                    b_in->get_lower_left_x() < s->get_right_x()
                );
            }
        );
        if (it_segment != it->second.end()) {
            if (b_in->get_upper_right_x() > (*it_segment)->get_right_x() - (*it_segment)->get_width()) {
                if (b_in->get_lower_left_x() > (*it_segment)->get_left_x()) {
                    (*it_segment)->set_right_x(b_in->get_lower_left_x());
                }
                else {
                    delete *it_segment;
                    it->second.erase(it_segment);
                }
            }
            else {
                double temp_left_x = (*it_segment)->get_left_x();
                temp_left_x += ceil((b_in->get_upper_right_x() - temp_left_x) / (*it_segment)->get_width()) * (*it_segment)->get_width();
                if (b_in->get_lower_left_x() > (*it_segment)->get_left_x()) {
                    segment* s = new segment();
                    s->set_left_x((*it_segment)->get_left_x());
                    s->set_right_x(b_in->get_lower_left_x());
                    s->set_width((*it_segment)->get_width());
                    (*it_segment)->set_left_x(temp_left_x);
                    it->second.insert(it_segment, s);
                }
                else {
                    (*it_segment)->set_left_x(temp_left_x);
                }
            }
        }
        it = this->segment_map.upper_bound(it->first);
    } while (it != this->segment_map.end() && b_in->get_upper_right_y() > it->first);
}

void optimizer_legalizer::create_segments_and_merge(block* b_in) {
    std::map<double, std::vector<segment*>>::iterator it = this->segment_map.lower_bound(b_in->get_lower_left_y());
    if (it->first != b_in->get_lower_left_y()) {
        --it;
    }
    do {
        auto it_segment = std::find_if(
            it->second.begin(),
            it->second.end(),
            [&b_in](const segment* s) {
                return (
                    b_in->get_lower_left_x() == s->get_right_x() ||
                    (b_in->get_upper_right_x() <= s->get_left_x() && (b_in->get_upper_right_x() > (s->get_left_x() - s->get_width())))
                );
            }
        );
        if (it_segment == it->second.end()) {
            segment* s = new segment();
            s->set_left_x(b_in->get_lower_left_x());
            if (it->second.empty()) {
                s->set_width(DEFAULT_WIDTH);
            }
            else {
                s->set_width(it->second[0]->get_width());
            }
            s->set_right_x(s->get_left_x() + ceil(b_in->get_width() / s->get_width()) * s->get_width());
            auto it_between = std::upper_bound(
                it->second.begin(),
                it->second.end(),
                s,
                [](const segment* a, const segment* b) {
                    return (a->get_left_x() < b->get_left_x());
                }
            );
            it->second.insert(it_between, s);
        }
        else {
            if ((*it_segment)->get_right_x() == b_in->get_lower_left_x()) {
                auto it_segment_next = it_segment;
                (*it_segment)->set_right_x((*it_segment)->get_right_x() + ceil(b_in->get_width() / (*it_segment)->get_width()) * (*it_segment)->get_width());
                std::advance(it_segment_next, 1);
                if (it_segment_next != it->second.end()) {
                    if ((*it_segment_next)->get_left_x() == (*it_segment)->get_right_x()) {
                        (*it_segment_next)->set_left_x((*it_segment)->get_left_x());
                        delete *it_segment;
                        it->second.erase(it_segment);
                    }
                }
            }
            else {
                (*it_segment)->set_left_x(b_in->get_lower_left_x());
            }
        }
        it = this->segment_map.upper_bound(it->first);
    } while (it != this->segment_map.end() && b_in->get_upper_right_y() > it->first);
}
