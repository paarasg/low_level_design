#include <Ilimiter.h>
#include <Iclock.h>
#include <chrono>
#include <memory>
#include <mutex>

class TokenBucket : public Ilimiter {
public:
    TokenBucket(double capacity, double rate_limit,
                std::shared_ptr<Iclock> clock = std::make_shared<SteadyClock>())
        : capacity_(capacity),
          rate_limit_(rate_limit),
          curr_tokens_(capacity),
          last_refill_(clock->Now()),
          clock_(std::move(clock)) {}
    
    bool Allow() override {
        std::lock_guard<std::mutex> lock(mu_);
        Refill();
        if (curr_tokens_ >= 1.0) {
            curr_tokens_ -= 1.0;
            return true;
        }
        return false;
    }

    bool TryReserve(int tokens) {
        if (tokens <= 0) return false;
        std::lock_guard<std::mutex> lock(mu_);
        Refill();
        if (curr_tokens_ >= (tokens * 1.0)) {
            curr_tokens_ -= (tokens * 1.0);
            return true;
        }
        return false;
    }

private:

    void Refill(void) {
        auto now = clock_->Now();
        double elapsed = std::chrono::duration<double>(now - last_refill_).count();
        if (elapsed > 0.0) {
            curr_tokens_ = std::min(capacity_, curr_tokens_ + (elapsed * rate_limit_));
            last_refill_ = now;
        }
    }

    double capacity_;
    double rate_limit_;
    double curr_tokens_;
    std::chrono::steady_clock::time_point last_refill_;
    std::shared_ptr<Iclock> clock_;
    std::mutex mu_;
};