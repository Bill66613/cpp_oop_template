/**
 * @file main.cc
 * @author Bill (ntmh.bill@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-06-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>

#include <helper/helper.hh>
#include <helper/safe_queue.hh>
#include <animal/animal.hh>
#include <sqlite/sqlite.hh>

#ifndef PROJECT_NAME
#define PROJECT_NAME "Test Project"
#endif

#ifndef PROJECT_VERSION
#define PROJECT_VERSION "0.0.0"
#endif

void InitializeProject()
{
  std::cout << "\n===================" << PROJECT_NAME << " " << PROJECT_VERSION << "===================\n" << std::endl;
}

int main(int argc, char const *argv[])
{
  InitializeProject();
  Animal friendly_dog("Sammy the Border Collie");
  friendly_dog.Greet();

  SQLiteTest();

  SingletonTest();
  return 0;
}
