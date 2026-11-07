#include <iostream>
#include "rate_limiter.hpp"
#include <cassert>
#include <chrono>
#include <thread>
#include "../include/rate_limiter.hpp"

using namespace rate_limiter;

void test_token_bucket_basic() {
    RateLimiter limiter(Algorithm::TOKEN_BUCKET, 3);
    assert(limiter.allow() == true);
    assert(limiter.allow() == true);
    assert(limiter.allow() == true);
    assert(limiter.allow() == false); // No tokens left
}

void test_sliding_window_basic() {
    RateLimiter limiter(Algorithm::SLIDING_WINDOW, 2, std::chrono::milliseconds(100));
    assert(limiter.allow() == true);
    assert(limiter.allow() == true);
    assert(limiter.allow() == false); // No more requests allowed
}

void test_token_bucket_reset() {
    RateLimiter limiter(Algorithm::TOKEN_BUCKET, 2);
    assert(limiter.allow() == true);
    assert(limiter.allow() == true);
    assert(limiter.allow() == false);
    limiter.reset();
    assert(limiter.allow() == true); // Should be allowed after reset
}

void test_sliding_window_reset() {
    RateLimiter limiter(Algorithm::SLIDING_WINDOW, 2, std::chrono::milliseconds(100));
    assert(limiter.allow() == true);
    assert(limiter.allow() == true);
    assert(limiter.allow() == false);
    limiter.reset();
    assert(limiter.allow() == true); // Should be allowed after reset
}

void test_determinism() {
    RateLimiter limiter1(Algorithm::TOKEN_BUCKET, 2);
    RateLimiter limiter2(Algorithm::TOKEN_BUCKET, 2);
    
    bool result1 = limiter1.allow();
    bool result2 = limiter2.allow();
    assert(result1 == result2); // Same initial state
}

void test_different_algorithms() {
    RateLimiter tb(Algorithm::TOKEN_BUCKET, 1);
    RateLimiter sw(Algorithm::SLIDING_WINDOW, 1, std::chrono::milliseconds(50));
    
    assert(tb.allow() == true);
    assert(sw.allow() == true);
    
    assert(tb.allow() == false); // No tokens
    assert(sw.allow() == false); // Window full
}

int main() {
    test_token_bucket_basic();
    test_sliding_window_basic();
    test_token_bucket_reset();
    test_sliding_window_reset();
    test_determinism();
    test_different_algorithms();
    
    std::cout << "All tests passed!\n";
    return 0;
}