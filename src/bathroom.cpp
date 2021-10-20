#include "../include/lab4.hpp"

void bathroom_t::use(person_t& p) {
    state_mtx_.lock();
    if (state_ == empty) {
        if (p.gender_ == "man") {
            state_ = occupied_by_mans;
            wash_mtx_.lock();
        }
        else if (p.gender_ == "woman") {
            state_ = occupied_by_womans;
            wash_mtx_.lock();
        }
    }
    else if (state_ == occupied_by_mans && p.gender_ == "woman" ||
        state_ == occupied_by_womans && p.gender_ == "man") {
        printf("%d %s waiting for mans to end washing\n", p.id_, p.gender_.c_str());
        state_mtx_.unlock();
        wash_mtx_.lock();
        wash_mtx_.unlock();
        state_mtx_.lock();
    }
    persons_in_++;
    state_mtx_.unlock();
    printf("%d %s started using bathroom\n", p.id_, p.gender_.c_str());
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    printf("%d %s stopped using bathroom\n", p.id_, p.gender_.c_str());
    state_mtx_.lock();
    persons_in_--;
    if (persons_in_ == 0) {
        state_ = empty;
        wash_mtx_.unlock();
    }
    state_mtx_.unlock();
}
