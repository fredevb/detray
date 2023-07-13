/** Detray library, part of the ACTS project (R&D line)
 *
 * (c) 2020-2023 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#pragma once

// Project include(s)
#include "detray/coordinates/cartesian2.hpp"
#include "detray/definitions/qualifiers.hpp"
#include "detray/intersection/plane_intersector.hpp"
#include "detray/surface_finders/grid/detail/axis_binning.hpp"
#include "detray/surface_finders/grid/detail/axis_bounds.hpp"

// System include(s)
#include <cmath>
#include <limits>
#include <string>

namespace detray {

/// @brief Geometrical shape of a rectangle2D.
///
/// @tparam intersector_t defines how to intersect the underlying surface
///         geometry
/// @tparam kMeasDim defines the dimension of the measurement
/// @tparam kNormalOrder true if the index for measurement parameter follows
/// the local coordinate system
///
/// It is defined by half length in local0 coordinates bounds[0] and bounds[1]
template <template <typename> class intersector_t = plane_intersector,
          unsigned int kMeasDim = 2u, bool kNormalOrder = true>
class rectangle2D {
    public:
    /// The name for this shape
    inline static const std::string name = "rectangle2D";

    /// The measurement dimension
    inline static constexpr const unsigned int meas_dim{kMeasDim};

    /// Normal ordering
    inline static constexpr const bool normal_order{kNormalOrder};

    // Measurement dimension check
    static_assert(meas_dim == 1u || meas_dim == 2u,
                  "Only 1D or 2D measurement is allowed");

    enum boundaries : unsigned int {
        e_half_x = 0u,
        e_half_y = 1u,
        e_size = 2u,
    };

    /// Local coordinate frame for boundary checks
    template <typename algebra_t>
    using local_frame_type = cartesian2<algebra_t>;

    /// Underlying surface geometry: planar
    template <typename intersection_t>
    using intersector_type = intersector_t<intersection_t>;

    /// Behaviour of the two local axes (linear in x, y)
    template <
        n_axis::bounds e_s = n_axis::bounds::e_closed,
        template <typename, typename> class binning_loc0 = n_axis::regular,
        template <typename, typename> class binning_loc1 = n_axis::regular>
    struct axes {
        static constexpr n_axis::label axis_loc0 = n_axis::label::e_x;
        static constexpr n_axis::label axis_loc1 = n_axis::label::e_y;
        static constexpr std::size_t dim{2u};

        /// How to convert into the local axis system and back
        template <typename algebra_t>
        using coordinate_type = local_frame_type<algebra_t>;

        using types = dtuple<n_axis::bounds_t<e_s, axis_loc0>,
                             n_axis::bounds_t<e_s, axis_loc1>>;

        template <typename C, typename S>
        using binning = dtuple<binning_loc0<C, S>, binning_loc1<C, S>>;
    };

    /// @brief Check boundary values for a local point.
    ///
    /// @note the point is expected to be given in local coordinates by the
    /// caller. For the conversion from global cartesian coordinates, the
    /// nested @c shape struct can be used.
    ///
    /// @param bounds the boundary values for this shape
    /// @param loc_p the point to be checked in the local coordinate system
    /// @param tol dynamic tolerance determined by caller
    ///
    /// @return true if the local point lies within the given boundaries.
    template <template <typename, std::size_t> class bounds_t,
              typename scalar_t, std::size_t kDIM, typename point_t,
              typename std::enable_if_t<kDIM == e_size, bool> = true>
    DETRAY_HOST_DEVICE inline bool check_boundaries(
        const bounds_t<scalar_t, kDIM>& bounds, const point_t& loc_p,
        const scalar_t tol = std::numeric_limits<scalar_t>::epsilon()) const {
        return (std::abs(loc_p[0]) <= bounds[e_half_x] + tol and
                std::abs(loc_p[1]) <= bounds[e_half_y] + tol);
    }

    /// @brief Lower and upper point for minimal axis aligned bounding box.
    ///
    /// Computes the min and max vertices in a local cartesian frame.
    ///
    /// @param bounds the boundary values for this shape
    /// @param env dynamic envelope around the shape
    ///
    /// @returns an array of coordinates that contains the lower point (first
    /// three values) and the upper point (latter three values) .
    template <typename algebra_t,
              template <typename, std::size_t> class bounds_t,
              typename scalar_t, std::size_t kDIM,
              typename std::enable_if_t<kDIM == e_size, bool> = true>
    DETRAY_HOST_DEVICE inline std::array<scalar_t, 6> local_min_bounds(
        const bounds_t<scalar_t, kDIM>& bounds,
        const scalar_t env = std::numeric_limits<scalar_t>::epsilon()) const {
        assert(env > 0.f);
        const scalar_t x_bound{bounds[e_half_x] + env};
        const scalar_t y_bound{bounds[e_half_y] + env};
        return {-x_bound, -y_bound, -env, x_bound, y_bound, env};
    }

    /// @brief Calculates the coordinates of the vertices.
    ///
    /// @param bounds the boundary values for this shape.
    ///
    /// @returns a container of vertices in clockwise order.
    template <typename point3_container_t,
              template <typename, std::size_t> class bounds_t,
              typename scalar_t,
              std::size_t kDIM,
              typename std::enable_if_t<kDIM == e_size, bool> = true>
    DETRAY_HOST inline point3_container_t local_vertices(
        const bounds_t<scalar_t, kDIM>& bounds) const {
        using point3_t = typename point3_container_t::value_type;
        point3_t v1 = {bounds[e_half_x], bounds[e_half_y], 0};
        point3_t v2 = {bounds[e_half_x], -bounds[e_half_y], 0};
        point3_t v3 = {-bounds[e_half_x], -bounds[e_half_y], 0};
        point3_t v4 = {-bounds[e_half_x], bounds[e_half_y], 0};
        return { v1, v2, v3, v4 };
    }
    
};

}  // namespace detray
