#include <FakeClock.h>
#include <TokenBucket.h>
#include <assert.h>
#include <iostream>
#include <thread>
#include <atomic>


static void generate_request(size_t &request_per_thread, TokenBucket &T, std::atomic<int> &allowed_cnt) {
    for (size_t i = 0; i < request_per_thread; i++) {
        if (T.Allow()) {
            allowed_cnt++;
        }
    }
}

static void test_multi_threaded_safe(void) {
    auto clock = std::make_shared<FakeClock>();
    TokenBucket T(10.0, 5.0, clock);
    size_t num_threads = 10;
    size_t requests_per_thread = 5;
    std::vector<std::thread> threads;
    std::atomic<int> allowed_cnt(0);

    for (size_t cnt = 0; cnt < num_threads; cnt++) {
        threads.emplace_back([&]{
            generate_request(requests_per_thread, T, allowed_cnt);
        });
    }

    for (auto &t : threads) {
        t.join();       
    }
    // std::cout << allowed_cnt.load() << std::endl;
    assert(allowed_cnt.load() == 10);

    threads.clear();

    clock->Advance(std::chrono::seconds(1));

    for (size_t i = 0; i < num_threads; i++) {
        threads.emplace_back([&]{
            generate_request(requests_per_thread, T, allowed_cnt);
        });
    }

    for (auto &t : threads) {
        t.join();       
    }

    assert(allowed_cnt.load() == 15);
}

static void test_basic(void) {
    auto clock = std::make_shared<FakeClock>();
    TokenBucket Ratelimiter(4.0, 2.0, clock);
    assert(Ratelimiter.Allow());
    assert(Ratelimiter.Allow());
    assert(Ratelimiter.Allow());
    assert(Ratelimiter.Allow());

    assert(!Ratelimiter.Allow());

    clock->Advance(std::chrono::seconds(1));

    assert(Ratelimiter.Allow());
    assert(Ratelimiter.Allow());

    assert(!Ratelimiter.Allow());
}

void test_try_reserve_tokens(void) {
    auto clock = std::make_shared<FakeClock>();
    TokenBucket T(10.0, 5.0, clock);

    assert(T.TryReserve(10));
    assert(!T.Allow());

    clock->Advance(std::chrono::seconds(1));
    assert(!T.TryReserve(10));

    for (size_t i = 0; i < 5; i++) {
        assert(T.Allow());
    }
    assert(!T.Allow());
}

int main() {
    test_basic();
    test_multi_threaded_safe();
    test_try_reserve_tokens();
    std::cout << "All unit tests passed.\n";
    return 0;
}