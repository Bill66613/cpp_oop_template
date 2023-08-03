#pragma once

#include <mutex>
#include <string>

template <typename T>
class Singleton
{
  /**
   * The Singleton's constructor/destructor should always be private to
   * prevent direct construction/desctruction calls with the `new`/`delete`
   * operator.
   */

public:
  static T &GetInstance()
  {
    static T instance_;
    return instance_;
  }

protected:
  Singleton() {}
  ~Singleton() {}

public:
  /**
   * Singletons should not be cloneable.
   */
  Singleton(Singleton &) = delete;
  Singleton(Singleton const &) = delete;
  Singleton &operator=(Singleton const &) = delete;
};
