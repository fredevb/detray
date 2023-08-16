/** Detray library, part of the ACTS project (R&D line)
 *
 * (c) 2023 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#pragma once

// Project inlude(s)
#include "detray/plugins/svgtools/meta/proto/landmark.hpp"

// Actsvg include(s)
#include "actsvg/core.hpp"

// System include(s)
#include <vector>
#include <string>

namespace detray::svgtools::meta::proto {

/// @brief A proto intersection record class as a simple translation layer from a intersection record description.
template <typename point3_t>
class intersection_record{
    public:
    using landmark_type = svgtools::meta::proto::landmark<point3_t>;
    std::vector<landmark_type> _landmarks;
    std::string _name;
};
}