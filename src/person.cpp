#include "../include/lab4.hpp"

person_t::person_t() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1);
    if (distrib(gen) == 0) {
        gender_ = "man";
    }
    else {
        gender_ = "woman";
    }
    id_ = id_counter.load();
    id_counter.fetch_add(1);
}
