#!/bin/bash

#/!\ Install script for macOS

# Check if Homebrew is installed
if ! command -v brew &> /dev/null; then
  echo -e "\033[1;30;41mHomebrew is not installed. Please install Homebrew first: https://brew.sh/\033[0m"
  exit 1
fi

# Update Homebrew
echo -e "\033[1;30;43mUpdating Homebrew...\033[0m"
brew update && brew upgrade
if [ $? -eq 0 ]; then
  echo -e "\033[1;30;42mHomebrew updated successfully.\033[0m"
else
  echo -e "\033[1;30;41mFailed to update Homebrew.\033[0m"
  exit 1
fi

# Install required dependencies
echo -e "\033[1;30;43mInstalling GLFW, and Assimp...\033[0m"
brew install glfw assimp
if [ $? -eq 0 ]; then
  echo -e "\033[1;30;42mGLFW, and Assimp installed successfully.\033[0m"
else
  echo -e "\033[1;30;41mFailed to install dependencies.\033[0m"
  exit 1
fi

echo -e "\033[1;30;42mInstallation complete.\033[0m"
