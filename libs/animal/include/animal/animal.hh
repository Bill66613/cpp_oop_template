#pragma once

#include <string>

class Animal
{
private:
  /* data */
  std::string name_;

public:
  Animal(std::string name);
  ~Animal();

  void Greet();
};

