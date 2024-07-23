#pragma once

#include <string>

#ifdef __APPLE__
#include <sys/stat.h> // chmod
#endif

// extract a zip & use download function to put in a specific path
auto extract_zip(const std::string &zip_url, const std::string &extract_path,
                 const std::string &version) -> bool;