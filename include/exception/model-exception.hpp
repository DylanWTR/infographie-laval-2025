#pragma once

#include <string>
#include <exception>

class ModelException : public std::exception {
  std::string _reason;

public:
  explicit ModelException(const std::string& reason): _reason(reason) {}

  const char *what() const noexcept override {
    return _reason.c_str();
  }
};