#pragma once

#include "safe_queue.hh"
#include "singleton.hh"

class SingletonTestClass : public Singleton<SingletonTestClass>
{
  friend class Singleton<SingletonTestClass>;

protected:
  std::string value_;

private:
  SingletonTestClass();

public:
  void Test();
  void SetValue(std::string value);
  std::string GetValue() const;
};

void SingletonTest();
