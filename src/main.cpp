#include "rate_limiter.hpp"
#include <iostream>
#include "../include/rate_limiter.hpp"

int main() {
    using namespace rate_limiter;

    std::cout << "Rate Limiter Demo\n";
    std::cout << "==================\n";

    // Test Token Bucket algorithm
    RateLimiter tb(Algorithm::TOKEN_BUCKET, 5, std::chrono::seconds(1));
    std::cout << "Token Bucket (5 req/sec):\n";
    for (int i = 0; i < 8; ++i) {
        if (tb.allow()) {
            std::cout << "  Request " << i+1 << " allowed\n";
        } else {
            std::cout << "  Request " << i+1 << " denied\n";
        }
    }

    // Test Sliding Window algorithm
    RateLimiter sw(Algorithm::SLIDING_WINDOW, 3, std::chrono::seconds(2));
    std::cout << "\nSliding Window (3 req/2sec):\n";
    for (int i = 0; i < 6; ++i) {
        if (sw.allow()) {
            std::cout << "  Request " << i+1 << " allowed\n";
        } else {
            std::cout << "  Request " << i+1 << " denied\n";
        }
    }

    return 0;
}