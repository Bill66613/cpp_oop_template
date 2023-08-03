#include <thread>
#include <iostream>

#include "helper.hh"

SingletonTestClass::SingletonTestClass()
{
  printf("SingletonTestClass constructor\n");
}

void SingletonTestClass::Test()
{
  printf("SingletonTestClass::Test()\n");
}

void SingletonTestClass::SetValue(std::string value)
{
  value_ = value;
}
std::string SingletonTestClass::GetValue() const
{
  return value_;
}

void ThreadFoo()
{
  // Following code emulates slow initialization.
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  SingletonTestClass &singleton = SingletonTestClass::GetInstance();
  singleton.SetValue("FOO");
  std::cout << &singleton << ": " << singleton.GetValue() << "\n";
  singleton.Test();
}

void ThreadBar()
{
  // Following code emulates slow initialization.
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  SingletonTestClass &singleton = SingletonTestClass::GetInstance();
  singleton.SetValue("BAR");
  std::cout << &singleton << ": " << singleton.GetValue() << "\n";
  singleton.Test();
}

void SingletonTest()
{
  std::thread t1(ThreadFoo);
  std::thread t2(ThreadBar);
  t1.join();
  t2.join();
}
