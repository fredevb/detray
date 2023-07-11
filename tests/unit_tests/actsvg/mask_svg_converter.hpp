#include <vector>
#include <type_traits>

#include "detray/geometry/surface.hpp"
#include "actsvg/meta.hpp"
#include "actsvg/proto/surface.hpp"

using namespace actsvg;

namespace detray_actsvg_conversion {

    using point3 = std::array<scalar, 3>;
    using point3_container = std::vector<point3>;
    using proto_surface = proto::surface<point3_container>;

    template <class mask_t>
    inline proto_surface convert_mask(const mask_t& m){
        proto_surface p_surface;

        const auto& shape = m.get_shape();

        constexpr auto is_annulus2D = std::is_same_v<decltype(m), const detray::mask<detray::annulus2D<>>&>;
        constexpr auto is_cylinder2D = std::is_same_v<decltype(m), const detray::mask<detray::cylinder2D<>>&>;
        constexpr auto is_rectangle2D = std::is_same_v<decltype(m), const detray::mask<detray::rectangle2D<>>&>;
        constexpr auto is_ring2D = std::is_same_v<decltype(m), const detray::mask<detray::ring2D<>>&>;
        constexpr auto is_trapezoid2D = std::is_same_v<decltype(m), const detray::mask<detray::trapezoid2D<>>&>;

        // Set bounds.
        if constexpr (is_annulus2D)
        {
            auto ri = static_cast<scalar>(m[shape.e_min_r]);
            auto ro = static_cast<scalar>(m[shape.e_max_r]);

            p_surface._type = proto_surface::type::e_annulus;
            p_surface._radii = {ri, ro};
        }

        else if constexpr (is_cylinder2D)
        {
            auto r = static_cast<scalar>(m[m.get_shape().e_r]);
            auto nhz = static_cast<scalar>(m[shape.e_n_half_z]);
            auto phz = static_cast<scalar>(m[shape.e_p_half_z]);
            
            p_surface._type = proto_surface::type::e_cylinder;
            p_surface._radii = {0., r};
            p_surface._zparameters = {-nhz, -phz};
        }

        else if constexpr (is_ring2D)
        {
            auto ri = static_cast<scalar>(m[shape.e_inner_r]);
            auto ro = static_cast<scalar>(m[shape.e_outer_r]);

            p_surface._type = proto_surface::type::e_disc;
            p_surface._radii = {ri, ro};
        }

        else if constexpr (is_rectangle2D)
        {
            p_surface._type = proto_surface::type::e_polygon;
        }

        else if constexpr (is_trapezoid2D)
        {
            p_surface._type = proto_surface::type::e_polygon;
        }

        // Set vertices.
        auto detray_vertices = m.local_vertices();            
        point3_container actsvg_vertices;
        for (auto dv : detray_vertices)
        {
            point3 av = {static_cast<scalar>(dv[0]), static_cast<scalar>(dv[1]), static_cast<scalar>(dv[2])};
            actsvg_vertices.push_back(av);
        }
        p_surface._vertices = actsvg_vertices;

        return p_surface;
    }
}