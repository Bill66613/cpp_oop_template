
#include <iostream>

#include "animal.hh"

Animal::Animal(std::string name)
    : name_(name)
{
}

Animal::~Animal()
{
}

void Animal::Greet()
{
  std::cout << "From " << name_ << " with love" << std::endl;
}
