#include <Iclock.h>
#include <chrono>

class FakeClock : public Iclock {
public:
    FakeClock(std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now())
        : now_(start) {}

    std::chrono::steady_clock::time_point Now() const override {
        return now_;
    }

    void Advance(std::chrono::seconds secs) { now_ += secs; }

    void Advance(std::chrono::milliseconds ms) { now_ += ms; }

private:
    std::chrono::steady_clock::time_point now_;
};
