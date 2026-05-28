#pragma once

#include <mutex>
#include <string>

#include "safe_queue.hh"
#include "singleton.hh"

class SingletonTestClass : public Singleton<SingletonTestClass>
{
  friend class Singleton<SingletonTestClass>;

private:
  SingletonTestClass();

  mutable std::mutex value_mutex_;
  std::string value_;

public:
  void Test();
  void SetValue(std::string value);
  std::string GetValue() const;
};

void SingletonTest();
