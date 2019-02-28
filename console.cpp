#include <chrono>
#include <ctime>
#include <iostream>

const char* date()
{
  auto time = std::chrono::system_clock::now();
  std::time_t end_time = std::chrono::system_clock::to_time_t(time);

  return std::ctime(&end_time);
}

void info(const char text[100])
{
  std::cout << "[" << date() << "]" << "[info] " << text << std::endl;
}

void error(const char text[100])
{
  std::cout << date() << "[ERROR] " << text << std::endl;
}
