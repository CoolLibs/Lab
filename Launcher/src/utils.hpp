#pragma once

#include <sys/stat.h>
#include <vector>

// Callback function to write data to a file
auto write_memory_callback(void *contents, size_t size, size_t nmemb,
                           std::vector<char> *userp) -> size_t;
auto string_write_callback(void *ptr, size_t size, size_t nmemb,
                           void *userdata) -> size_t;

//get the OS
auto get_OS() -> std::string;
auto get_PATH() -> std::string;
