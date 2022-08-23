/** Algebra plugins library, part of the ACTS project
 *
 * (c) 2022 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#pragma once

// Project include(s).
#include "detray/coordinates/coordinate_base.hpp"
#include "detray/definitions/qualifiers.hpp"

namespace detray {

/** Frame projection into a cartesian coordinate frame
 */
template <typename transform3_t>
struct cartesian3 final : public coordinate_base<cartesian3, transform3_t> {

    /// @name Type definitions for the struct
    /// @{

    /// Base type
    using base_type = coordinate_base<cartesian3, transform3_t>;
    /// Sclar type
    using scalar_type = typename base_type::scalar_type;
    /// Point in 2D space
    using point2 = typename base_type::point2;
    /// Point in 3D space
    using point3 = typename base_type::point3;
    /// Vector in 3D space
    using vector3 = typename base_type::vector3;

    /// @}

    /** This method transform from a point from 3D cartesian frame to a 3D
     * cartesian point */
    DETRAY_HOST_DEVICE
    inline point3 operator()(const point3 &local3) const { return local3; }

    /** This method transform from a point from global cartesian 3D frame to a
     * local 3D cartesian point */
    DETRAY_HOST_DEVICE
    inline point3 global_to_local(const transform3_t &trf, const point3 &p,
                                  const vector3 & /*d*/) const {
        return trf.point_to_local(p);
    }

    /** This method transform from a local 3D cartesian point to a point global
     * cartesian 3D frame*/
    template <typename mask_t>
    DETRAY_HOST_DEVICE inline point3 local_to_global(
        const transform3_t &trf, const mask_t & /*mask*/, const point3 &p,
        const vector3 & /*d*/) const {
        return trf.point_to_global(p);
    }

};  // struct cartesian3

}  // namespace detray