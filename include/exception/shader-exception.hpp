#pragma once

#include <string>
#include <exception>

class ShaderException : public std::exception {
  std::string _reason;

public:
  explicit ShaderException(const std::string& reason): _reason(reason) {}

  const char *what() const noexcept override {
    return _reason.c_str();
  }
};