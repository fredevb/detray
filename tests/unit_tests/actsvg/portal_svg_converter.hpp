#include <vector>
#include <type_traits>

#include "detray/geometry/surface.hpp"
#include "actsvg/meta.hpp"
#include "actsvg/proto/surface.hpp"
#include "./surface_svg_converter.hpp"

using namespace actsvg;

namespace detray_actsvg_conversion{

    using point3 = std::array<scalar, 3>;
    using point3_container = std::vector<point3>;
    using proto_surface = proto::surface<point3_container>;
    using proto_portal = proto::portal<point3_container>;
    using proto_link = proto_portal::link;

    template <typename link_t>
    proto_link convert_link(link_t d_link)
    {
        proto_link p_link;
    }

    template <typename portal_t, typename context_t>
    proto_surface convert_portal(portal_t d_portal, context_t context)
    {
        proto_portal p_portal;
        p_portal._surface = convert_surface(d_portal, context);
        p_portal._volume_links = 0;


    }
}