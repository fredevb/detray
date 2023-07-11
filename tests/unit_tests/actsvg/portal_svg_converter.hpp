#include <vector>
#include <type_traits>

#include "detray/geometry/surface.hpp"
#include "actsvg/meta.hpp"
#include "actsvg/proto/surface.hpp"
#include "./mask_svg_converter.hpp"

using namespace actsvg;

namespace detray_actsvg_conversion{

    using point3 = std::array<scalar, 3>;
    using point3_container = std::vector<point3>;
    using proto_surface = proto::surface<point3_container>;

}