#include <cstddef>
#include "utils.hpp"

// Vérifier les permissions sur le dossier /Applications

auto write_memory_callback(void* contents, size_t size, size_t nmemb, std::vector<char>* userp) -> size_t
{
    size_t realsize = size * nmemb;
    userp->insert(userp->end(), (char*)contents, (char*)contents + realsize);
    return realsize;
}

// Callback function to write data to a string
auto string_write_callback(void* ptr, size_t size, size_t nmemb, void* userdata) -> size_t
{
    std::string* str = static_cast<std::string*>(userdata);
    str->append(static_cast<char*>(ptr), size * nmemb);
    return size * nmemb;
}

auto get_OS() -> std::string
{
    // which OS ?
#ifdef __APPLE__
    return "MacOS";
#elif _WIN32
    return "Windows";
#elif __linux__
    return "Linux";
#else
    static_assert(false);
#endif
}

auto get_PATH() -> std::string
{
#ifdef __APPLE__
    return "/Applications/Coollab/";
#elif defined(_WIN32)
    return "C:\\Program Files\\Coolab\\";
#elif defined(__linux__)
    return "/usr/local/Coollab/";
#else
    static_assert(false);
#endif
}