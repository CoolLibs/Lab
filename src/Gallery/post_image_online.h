#pragma once
#include "ArtworkInfo.h"
#include "AuthorInfo.h"
#include "LegalInfo.h"

namespace Lab {

void post_image_online(ArtworkInfo const&, AuthorInfo const&, LegalInfo const&, std::string const& image_png_data);

} // namespace Lab