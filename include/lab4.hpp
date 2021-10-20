#pragma once

#include <string>
#include <random>
#include <atomic>
#include <atomic>
#include <mutex>

struct bathroom_t;
struct person_t;

static std::atomic<int> id_counter = 0;

enum state_t { empty, occupied_by_mans, occupied_by_womans };

struct bathroom_t {
    void use(person_t& person);
    int persons_in_ = 0;
    std::mutex state_mtx_;
    std::mutex wash_mtx_;
    state_t state_;
};

struct person_t {
    person_t();
    int id_;
    std::string gender_;
};

