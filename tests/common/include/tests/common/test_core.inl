/** Detray library, part of the ACTS project (R&D line)
 *
 * (c) 2020-2022 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#include <gtest/gtest.h>

#include <climits>
#include <cmath>

#include "detray/core/type_registry.hpp"
#include "detray/geometry/surface.hpp"
#include "detray/intersection/intersection.hpp"
#include "detray/masks/unmasked.hpp"

/// @note __plugin has to be defined with a preprocessor command

using namespace detray;

using transform3 = __plugin::transform3<detray::scalar>;

enum mask_ids : unsigned int {
    e_unmasked = 0,
};
using mask_defs = mask_registry<mask_ids, unmasked<>>;
using mask_link_t = typename mask_defs::link_type;

constexpr scalar epsilon = std::numeric_limits<scalar>::epsilon();

// This tests the construction of a surface_base object
TEST(ALGEBRA_PLUGIN, surface) {
    // Preparatioon work, create a transform
    vector3 z = vector::normalize(vector3{3., 2., 1.});
    vector3 x = vector::normalize(vector3{2., -3., 0.});
    point3 t{2., 3., 4.};
    transform3 trf(t, z, x);

    mask_link_t mask_id{mask_defs::id::e_unmasked, 0};
    surface<mask_defs, transform3> s(std::move(trf), std::move(mask_id), -1,
                                     false, false);
}

// This tests the construction of a intresection
TEST(ALGEBRA_PLUGIN, intersection) {

    intersection i0 = {2., point3{0.3, 0.5, 0.7}, point2{0.2, 0.4},
                       intersection_status::e_hit};

    intersection i1 = {1.7, point3{0.2, 0.3, 0.}, point2{0.2, 0.4},
                       intersection_status::e_inside};

    intersection invalid;
    ASSERT_TRUE(invalid.status == intersection_status::e_missed);

    dvector<intersection> intersections = {invalid, i0, i1};
    std::sort(intersections.begin(), intersections.end());

    ASSERT_NEAR(intersections[0].path, 1.7, epsilon);
    ASSERT_NEAR(intersections[1].path, 2, epsilon);
    ASSERT_TRUE(std::isinf(intersections[2].path));
}
