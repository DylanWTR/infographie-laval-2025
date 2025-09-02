#pragma once

#include <string>
#include <exception>

class TextureException : public std::exception {
  std::string _reason;

public:
  explicit TextureException(const std::string& reason): _reason(reason) {}

  const char *what() const noexcept override {
    return _reason.c_str();
  }
};