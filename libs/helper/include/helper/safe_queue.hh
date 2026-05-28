#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

// A thread-safe FIFO queue.
//
// push()        — enqueue and wake one waiting consumer
// try_pop()     — dequeue without blocking; returns std::nullopt if empty
// wait_and_pop()— block until an element is available, then dequeue
// empty() / size() — snapshot queries (consistent under the internal lock)
template <class T>
class SafeQueue
{
public:
  SafeQueue() = default;
  ~SafeQueue() = default;

  SafeQueue(const SafeQueue&) = delete;
  SafeQueue& operator=(const SafeQueue&) = delete;

  bool empty() const
  {
    std::lock_guard<std::mutex> lock(m_);
    return q_.empty();
  }

  std::size_t size() const
  {
    std::lock_guard<std::mutex> lock(m_);
    return q_.size();
  }

  void push(T value)
  {
    {
      std::lock_guard<std::mutex> lock(m_);
      q_.push(std::move(value));
    }
    cv_.notify_one();
  }

  // Non-blocking dequeue. Returns std::nullopt when the queue is empty.
  std::optional<T> try_pop()
  {
    std::lock_guard<std::mutex> lock(m_);
    if (q_.empty())
      return std::nullopt;
    T tmp = std::move(q_.front());
    q_.pop();
    return tmp;
  }

  // Blocking dequeue. Suspends the caller until an element is available.
  T wait_and_pop()
  {
    std::unique_lock<std::mutex> lock(m_);
    cv_.wait(lock, [this] { return !q_.empty(); });
    T tmp = std::move(q_.front());
    q_.pop();
    return tmp;
  }

private:
  std::queue<T>           q_;
  mutable std::mutex      m_;
  std::condition_variable cv_;
};