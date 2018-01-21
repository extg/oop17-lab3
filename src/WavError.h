#pragma once

#include <iostream>

class WavError : std::exception {
private:
  std::string cause;
public:
  explicit WavError(const std::string &c)
  {
    cause = c;
  }

  virtual const char* what() const throw()
  {
    return cause.c_str();
  }
};
