#include "extractor.hpp"
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>
#include "miniz.h"
#include "utils.hpp"

namespace fs = std::__fs::filesystem;

auto extract_zip(std::vector<char> const& zip, std::string_view const& version) -> bool
{
    // Télécharger le fichier zip en mémoire
    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));
    if (!mz_zip_reader_init_mem(&zip_archive, zip.data(), zip.size(), 0))
    {
        std::cerr << "Failed to initialize zip reader." << std::endl;
        return false;
    }

    // Créer le répertoire parent s'il n'existe pas
    std::string parent_folder = get_PATH() + std::string(version);
    if (!fs::exists(parent_folder))
    {
        if (!fs::create_directories(parent_folder))
        {
            std::cerr << "Failed to create parent directory: " << parent_folder
                      << std::endl;
            mz_zip_reader_end(&zip_archive);
            return false;
        }
    }

    // Pour chaque fichier dans le zip
    int num_files = mz_zip_reader_get_num_files(&zip_archive);
    for (int i = 0; i < num_files; ++i)
    {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat))
        {
            std::cerr << "Failed to get file info for index " << i << std::endl;
            continue;
        }

        // Exclure les fichiers _MACOSX
        const char* name = file_stat.m_filename;
        if (name && std::string(name).find("_MACOSX") != std::string::npos)
        {
            continue;
        }

        // Chemin complet du fichier extrait
        std::string full_path = parent_folder + "/" + name;

        // Créer les répertoires nécessaires
        fs::create_directories(fs::path(full_path).parent_path());

        // Lire les données du fichier
        std::vector<char> file_data(file_stat.m_uncomp_size);
        if (!mz_zip_reader_extract_to_mem(&zip_archive, i, file_data.data(), file_stat.m_uncomp_size, 0))
        {
            std::cerr << "Failed to read file data: " << name << std::endl;
            continue;
        }

        // Écrire les données dans le fichier sur le disque
        std::ofstream ofs(full_path, std::ios::binary);
        ofs.write(file_data.data(), file_data.size());
        ofs.close();

#ifdef __APPLE__
        if (chmod(full_path.c_str(), 0755) != 0)
        {
            std::cerr << "Failed to set permissions on file: " << full_path
                      << std::endl;
        }
#endif

        // Afficher le chemin du fichier créé
        std::cout << "Created file: " << full_path << std::endl;
    }

    // Fermer le zip
    mz_zip_reader_end(&zip_archive);
    return true;
}
