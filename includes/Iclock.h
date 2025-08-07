#pragma once
#include <chrono>

class Iclock {
public:
    virtual ~Iclock() = default;
    virtual std::chrono::steady_clock::time_point Now() const = 0;
};

class SteadyClock : public Iclock {
public:
    std::chrono::steady_clock::time_point Now() const override {
        return std::chrono::steady_clock::now();
    }
};