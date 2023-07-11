#include <vector>
#include <type_traits>

#include "detray/geometry/surface.hpp"
#include "actsvg/meta.hpp"
#include "actsvg/proto/surface.hpp"
#include "detray/definitions/units.hpp"
#include "./mask_svg_converter.hpp"

using namespace actsvg;

namespace detray_actsvg_conversion{

    using point3 = std::array<scalar, 3>;
    using point3_container = std::vector<point3>;
    using proto_surface = proto::surface<point3_container>;

    template <typename matrix_t>
    inline std::array<detray::scalar, 3> rotation_matrix_to_euler_angles(matrix_t mat){
        float a = sqrt(mat[0][0] * mat[0][0] + mat[1][0] * mat[1][0]);
        // Checking if it is singular.
        if (a < 1e-6)
            return {atan2(-mat[1][2], mat[1][1]), atan2(-mat[2][0], a), 0};

        return {atan2(mat[2][1], mat[2][2]), atan2(-mat[2][0], a), atan2(mat[1][0], mat[0][0])};
    }

    template <typename transform_t>
    inline auto convert_transform(transform_t d_transform){
        auto translation = d_transform.translation();
        auto euler_angles = rotation_matrix_to_euler_angles<>(d_transform.rotation());

        //TODO: skew and scale.

        auto ret = actsvg::style::transform();
        constexpr auto rad_to_deg = 180.0/3.14;
        ret._tr = {static_cast<scalar>(translation[0]), static_cast<scalar>(translation[1])};
        ret._rot = {
            //TODO: Check if this is the correct work around.
            static_cast<scalar>(euler_angles[2] * rad_to_deg),
            static_cast<scalar>(euler_angles[1] * rad_to_deg),
            static_cast<scalar>(euler_angles[0] * rad_to_deg)};

        return ret;
    }

    template <typename algebra_t = __plugin::transform3<detray::scalar>>
    struct to_proto_surface {

        template <typename mask_group_t, typename index_t>
        DETRAY_HOST inline proto::surface<point3_container> operator()(
            const mask_group_t& mask_group, 
            const index_t& index) const {
            const auto& m = mask_group[index];
            return convert_mask(m);
        }

    };
    
    template <typename surface_t, typename context_t>
    proto_surface convert_surface(const surface_t d_surface, const context_t context)
    {
        auto p_surface = d_surface.template visit_mask<to_proto_surface<>>();
        p_surface._transform = convert_transform(d_surface.transform(context));
        return p_surface;
    }
}