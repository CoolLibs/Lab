#include "extractor.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include "miniz.h"
#include "utils.hpp"

namespace fs = std::filesystem;

auto extract_zip(const std::vector<char>& zip, std::string_view const& version) -> void
{
    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));

    mz_zip_reader_init_mem(&zip_archive, zip.data(), zip.size(), 0);

    std::string parent_folder = get_PATH() + std::string(version);
    if (!fs::exists(parent_folder))
        fs::create_directories(parent_folder);

    int num_files = mz_zip_reader_get_num_files(&zip_archive);
    for (int i = 0; i < num_files; ++i)
    {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat))
        {
            std::cerr << "Failed to get file info for index " << i << std::endl;
            continue;
        }

        const char* name = file_stat.m_filename;
        if (name && std::string(name).find("_MACOSX") != std::string::npos)
        {
            continue;
        }

        std::string full_path = parent_folder + "/" + name;

        fs::create_directories(fs::path(full_path).parent_path());

        std::vector<char> file_data(file_stat.m_uncomp_size);
        if (!mz_zip_reader_extract_to_mem(&zip_archive, i, file_data.data(), file_stat.m_uncomp_size, 0))
        {
            std::cerr << "Failed to read file data: " << name << std::endl;
            continue;
        }

        std::ofstream ofs(full_path, std::ios::binary);
        if (!ofs)
        {
            std::cerr << "Failed to open file for writing: " << full_path << std::endl;
            continue;
        }
        ofs.write(file_data.data(), file_data.size());
        ofs.close();

#ifdef __APPLE__
        if (chmod(full_path.c_str(), 0755) != 0)
            std::cerr << "Failed to set permissions on file: " << full_path << std::endl;
#endif

        std::cout << "Created file: " << full_path << std::endl;

        mz_zip_reader_end(&zip_archive);
        return;
    }

    mz_zip_reader_end(&zip_archive);
}
