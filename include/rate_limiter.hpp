#pragma once
#include <chrono>
#include <cstdint>
#include <deque>
#include <mutex>
#include <atomic>

namespace rate_limiter {

enum class Algorithm { TOKEN_BUCKET, SLIDING_WINDOW };

class RateLimiter {
public:
    explicit RateLimiter(Algorithm algorithm, std::uint64_t max_requests, 
                        std::chrono::milliseconds window_size = std::chrono::seconds(1));

    bool allow();
    void reset();

private:
    Algorithm algorithm_;
    std::uint64_t max_requests_;
    std::chrono::milliseconds window_size_;

    // Token Bucket specific members
    mutable std::mutex token_mutex_;
    std::atomic<std::uint64_t> tokens_{};
    std::atomic<std::chrono::steady_clock::time_point> last_refill_{};

    // Sliding Window specific members
    mutable std::mutex window_mutex_;
    std::deque<std::chrono::steady_clock::time_point> requests_;
};

inline RateLimiter::RateLimiter(Algorithm algorithm, std::uint64_t max_requests,
                                std::chrono::milliseconds window_size)
    : algorithm_(algorithm),
      max_requests_(max_requests),
      window_size_(window_size),
      tokens_(max_requests),
      last_refill_(std::chrono::steady_clock::now()) {}

inline bool RateLimiter::allow() {
    switch (algorithm_) {
        case Algorithm::TOKEN_BUCKET: {
            std::lock_guard<std::mutex> lock(token_mutex_);
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_refill_.load()).count();

            if (elapsed >= window_size_.count()) {
                tokens_.store(max_requests_);
                last_refill_.store(now);
            } else {
                // Refill tokens based on time elapsed
                auto refill_tokens = static_cast<std::uint64_t>(elapsed * max_requests_ / window_size_.count());
                if (refill_tokens > 0) {
                    tokens_.fetch_add(refill_tokens);
                    last_refill_.store(now);
                }
            }

            auto current_tokens = tokens_.load();
            if (current_tokens > 0) {
                tokens_.fetch_sub(1);
                return true;
            }
            return false;
        }
        case Algorithm::SLIDING_WINDOW: {
            std::lock_guard<std::mutex> lock(window_mutex_);
            auto now = std::chrono::steady_clock::now();

            // Remove old requests outside the window
            while (!requests_.empty() && 
                   (now - requests_.front()) >= window_size_) {
                requests_.pop_front();
            }

            if (requests_.size() < max_requests_) {
                requests_.push_back(now);
                return true;
            }
            return false;
        }
    }
    return false; // Should never reach here
}

inline void RateLimiter::reset() {
    switch (algorithm_) {
        case Algorithm::TOKEN_BUCKET: {
            std::lock_guard<std::mutex> lock(token_mutex_);
            tokens_.store(max_requests_);
            last_refill_.store(std::chrono::steady_clock::now());
            break;
        }
        case Algorithm::SLIDING_WINDOW: {
            std::lock_guard<std::mutex> lock(window_mutex_);
            requests_.clear();
            break;
        }
    }
}

} // namespace rate_limiter