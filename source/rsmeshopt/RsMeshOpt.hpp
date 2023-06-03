#pragma once

#include <expected>
#include <span>
#include <stdint.h>
#include <string>
#include <vector>

#include "TriStripper/public_types.h"

namespace rsmeshopt {

std::expected<std::vector<std::vector<uint32_t>>, std::string>
StripifyTrianglesNvTriStripPort(std::span<const uint32_t> index_data);

std::expected<triangle_stripper::primitive_vector, std::string>
StripifyTrianglesTriStripper(std::span<const uint32_t> index_data);

} // namespace rsmeshopt
