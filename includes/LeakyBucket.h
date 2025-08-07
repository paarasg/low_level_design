#include <Ilimiter.h>
#include <Iclock.h>
#include <memory>

class LeakyBucket : public Ilimiter {
public:
    LeakyBucket(double capacity, double rate, std::shared_ptr<Iclock> clock = std::make_shared<SteadyClock>())
        : capacity_(capacity),
          rate_(rate),
          curr_capacity_(capacity),
          clock_(std::move(clock)),
          last_leaked_(clock_->Now()) {}

    bool Allow(void) override {
        std::lock_guard<std::mutex> lock(mu_);
        Leak();
        if (curr_capacity_ < capacity_) {
            curr_capacity_++;
            return true;
        }
        return false;
    }

private:
    void Leak(void) {
        std::chrono::steady_clock::time_point now = clock_->Now();
        auto elapsed = std::chrono::duration<double>(now - last_leaked_).count();
        if (elapsed > 0.0) {
            curr_capacity_ -= std::min(curr_capacity_, (elapsed * rate_));
            last_leaked_ =  now;
        }
    }

    double capacity_;
    double curr_capacity_;
    double rate_;
    std::shared_ptr<Iclock> clock_;
    std::chrono::steady_clock::time_point last_leaked_;
    std::mutex mu_;
};