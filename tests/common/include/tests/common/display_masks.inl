/** Detray library, part of the ACTS project (R&D line)
 * 
 * (c) 2021 CERN for the benefit of the ACTS project
 * 
 * Mozilla Public License Version 2.0
 */

#include "masks/masks.hpp"
#include "style/styles.hpp"
#include "view/views.hpp"
#include "view/draw.hpp"

#include <cmath>
#include <climits>

#include <gtest/gtest.h>

/// @note __plugin has to be defined with a preprocessor command
using namespace detray;

using transform3 = __plugin::transform3;
using vector3 = __plugin::transform3::vector3;
using point3 = __plugin::transform3::point3;


TEST(display, annulus2)
{
    detray::global_xy_view gxy;

    // First rectangle
    transform3 transform{};
    annulus2<> annulus = {7.2, 12.0, 0.74195, 1.33970, 0., -2., 2.};

    color c = {0.2, 0.8, 0.6, 0.9 };
    display(false);
    draw(annulus, transform, style{c}, gxy);
    save("annulus.png");

}

TEST(display, rectangle2)
{
    detray::global_xy_view gxy;

    // First rectangle
    transform3 transform{};
    rectangle2<> rectangle = {3.,4.};

    color c = {0.5, 0.2, 0.6, 0.9 };
    display(false);
    draw(rectangle, transform, style{c}, gxy);
    save("rectangle.png");

}


TEST(display, trapezoid2)
{
    detray::global_xy_view gxy;

    // First rectangle
    transform3 transform{};
    trapezoid2<> trapezoid = {3.,4.,5.};

    color c = {0.5, 0.4, 0.4, 0.9 };
    display(false);
    draw(trapezoid, transform, style{c}, gxy);
    save("trapezoid.png");

}


// Google Test can be run manually from the main() function
// or, it can be linked to the gtest_main library for an already
// set-up main() function primed to accept Google Test test cases.
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
