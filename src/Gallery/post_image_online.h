#pragma once
#include "ArtworkInfo.h"
#include "AuthorInfo.h"

namespace Lab {

void post_image_online(ArtworkInfo const&, AuthorInfo const&, std::string const& image_png_data);

} // namespace Lab