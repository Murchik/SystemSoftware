#include <vector>
#include <thread>

#include "../include/lab4.hpp"

int main() {
    bathroom_t bathroom;
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; i++) {
        threads.emplace_back([&] {
            person_t person;
            bathroom.use(person);
            });
    }
    for (auto& thread : threads) {
        thread.join();
    }
    return 0;
}
