#!/bin/bash

# Save current working dir
execDir=$(pwd)

# Enviroment Variables
set -o allexport
        # the directory this script is located in
source "$(dirname $(realpath ${BASH_SOURCE[0]}))/.env"
set +o allexport

if [[ "$customCompile" == true ]]; then
  "$customCompileScript"
  exit 0
fi

# Echo some info
echo -e "${blue}-- Project: $projectName | Build Type: $buildType | Compiler: $compiler --${clear}"
echo -e "${blue}-- ProjectRoot: $projectDir | Compiling verbose: $compileVerbose | clean CMake:$cleanCMakeFiles --${clear}"
echo -e "${blue}-- Executable Name: $executableName | Executable Path: $executablePath --${clear}"

# Going to the project directory
cd "$projectDir" || (echo "$projectDir doesn\'t exist" && exit 1)

# Looking if the compiler exists
if ! command -v "$compiler" &>/dev/null; then
  echo -e "${red}Error: Compiler '$compiler' not found${clear}"
  exit 1
fi

# Cleaning Build
if [[ "$cleanBuild" == true ]]; then
  echo -e "${cyan}-- Cleaning $binDir --${clear}"
  rm -rf "$binDir"
  mkdir -p "$binDir"
fi

# Cleaning CMake Files
if [[ "$cleanCMakeFiles" == true ]]; then
  echo -e "${cyan}-- Cleaning $cmakeDir --${clear}"
  rm -rf "$cmakeDir"
  mkdir -p "$cmakeDir"
fi

# Generating CMake Files
if [[ "$generateCMakeFiles" == true ]]; then
  echo -e "${cyan}-- Generating CMake files --${clear}"
  cmake -B "$cmakeDir" -S cmake \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_CXX_COMPILER="$compiler" \
    -DCMAKE_BUILD_TYPE="$buildType" \
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="$binDir" \
    -DCMAKE_LIBRARY_OUTPUT_DIRECTORY="$binDir" \
    -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY="$binDir" \
    -G "$buildSystem"

  echo -e "${green}-- Done Generating CMake files -- ${clear}"
fi

# Linking compile_commands.json File
if [[ "$linkCompileCommands" == true ]] then
  # Looking if compile_commands.json exist
  if [[ !(-f "$cmakeDir/compile_commands.json") ]]; then
    echo -e "${red}-- Error: compile_commands.json not found in $cmakeDir --${clear}"
    exit 1
  fi

  # Link compile_commands.json to projectDir
  ln -sf "$cmakeDir/compile_commands.json" "$projectDir/compile_commands.json"
fi

# Build the project
if [[ "$compileProject" == true ]]; then
  echo -e "${cyan}-- Compiling the project --${clear}"
  if [[ "$compileVerbose" == true ]]; then
    echo -e "${magenta}   -> Compiling verbose${clear}"
    cmake --build "$cmakeDir" --verbose
  else
    cmake --build "$cmakeDir"
  fi

  echo -e "${green}-- Done Compiling $executableName --${clear}"
  echo -e "${blue}-- Executable found in $(dirname $executablePath) --${clear}"
fi

# Running
if [[ "$run" == true ]]; then
  echo -e "${cyan}-- Running $executableName --${clear}"
  cd "$binDir" || exit 1 
  "$executablePath"
  cd "$execDir" || exit 1
  echo -e "${yellow}-- Executable: \"$executableName\" located in \"$executablePath\" --${clear}"
  echo -e "${green}-- Done Running $executableName --${clear}"
fi
