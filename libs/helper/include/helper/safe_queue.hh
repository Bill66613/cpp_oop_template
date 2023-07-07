/**
 * @file SafeQueue.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-06-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef SAFE_QUEUE
#define SAFE_QUEUE

#include <queue>
#include <mutex>
#include <condition_variable>

// A threadsafe-queue.
template <class T>
class SafeQueue
{
private:
  std::queue<T> q;
  mutable std::mutex m;
  std::condition_variable c;

public:
  SafeQueue(void)
  : q()
  , m()
  , c()
  {}

  ~SafeQueue(void)
  {}

  bool empty() const
  {
    return q.empty();
  }

  unsigned long size() const
  {
    std::lock_guard<std::mutex> lock(m);
    return q.size();
  }

  T front()
  {
    std::lock_guard<std::mutex> lock(m);
    if (q.empty())
    {
      return {};
    }
    T tmp = q.front();
    return tmp;
  }

  T back()
  {
    std::lock_guard<std::mutex> lock(m);
    if (q.empty())
    {
      return {};
    }
    T tmp = q.back();
    return tmp;
  }

  void push(T t)
  {
    std::lock_guard<std::mutex> lock(m);
    q.push(t);
    c.notify_one();
  }

  T pop()
  {
    std::lock_guard<std::mutex> lock(m);
    if (q.empty())
    {
      return {};
    }
    T tmp = q.front();
    q.pop();
    return tmp;
  }
};

#endif