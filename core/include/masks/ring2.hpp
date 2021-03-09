/** Detray library, part of the ACTS project (R&D line)
 * 
 * (c) 2020 CERN for the benefit of the ACTS project
 * 
 * Mozilla Public License Version 2.0
 */
#pragma once

#include "masks/mask_identifier.hpp"
#include "core/intersection.hpp"
#include "utils/containers.hpp"
#include "tools/planar_intersector.hpp"

#include <cmath>
#include <climits>

namespace detray
{
    /** This is a simple 2-dimensional mask for a closed ring
     * 
     * @tparam intersector_type is a struct used for intersecting this cylinder
     * @tparam links_type is an object where the mask can link to 
     * @tparam kMaskIdentifier is a unique mask identifier in a program context
     *  
     * It is defined by the two radii _values[0] and  _values[1], 
     * and can be checked with a tolerance in t[0] and t[1].
     * 
     **/
    template <typename intersector_type = planar_intersector,
              typename links_type = bool,
              unsigned int kMaskIdentifier = e_ring2>
    struct ring2
    {
        using mask_tolerance = scalar;

        using mask_values = darray<scalar, 2>;

        mask_values _values = {0., std::numeric_limits<scalar>::infinity()};

        links_type _links;

        static constexpr unsigned int mask_identifier = kMaskIdentifier;

        static constexpr mask_tolerance within_epsilon 
            = std::numeric_limits<scalar>::epsilon();

        /** Assignment operator from an array, convenience function
         * 
         * @param rhs is the right hand side object
         **/
        ring2<intersector_type, links_type, kMaskIdentifier> &
        operator=(const darray<scalar, 2> &rhs)
        {
            _values = rhs;
            return (*this);
        }

        /** Mask operation 
         * 
         * @tparam point2_type is the type of the point to be checked w.r.t. to
         * the mask bounds
         * 
         * @param p the point to be checked
         * @param t is the tolerance in r
         * 
         * @return an intersection status e_inside / e_outside
         **/
        template<typename local_type>
        intersection_status is_inside(const typename local_type::point2 &p,
                                      const mask_tolerance &t = within_epsilon) const
        {
            if constexpr(std::is_same_v<local_type, __plugin::cartesian2>) {
               scalar r = getter::perp(p);
               return (r + t >= _values[0] and r <= _values[1] + t) ? e_inside : e_outside;
            }

            return (p[0] + t >= _values[0] and p[0] <= _values[1] + t) ? e_inside : e_outside;
        }


        /** Equality operator from an array, convenience function
         * 
         * @param rhs is the rectangle to be compared with
         * 
         * checks identity within epsilon and @return s a boolean*
         **/
        bool operator==(const darray<scalar, 2> &rhs)
        {
            return (_values == rhs);
        }

        /** Equality operator 
         * 
         * @param rhs is the rectangle to be compared with
         * 
         * checks identity within epsilon and @return s a boolean*
         **/
        bool operator==(const ring2<> &rhs)
        {
            return operator==(rhs._values);
        }

        /** Access operator - non-const
         * @return the reference to the member variable
         */
        scalar &operator[](unsigned int value_index)
        {
            return _values[value_index];
        }

        /** Access operator - non-const
         * @return a copy of the member variable
         */
        scalar operator[](unsigned int value_index) const
        {
            return _values[value_index];
        }

        /** Return an associated intersector type */
        intersector_type intersector() const { return intersector_type{}; };

        /** Return the volume link - const reference */
        const links_type &links() const { return _links; }

        /** Return the volume link - non-const access */
        links_type &links() { return _links; }

    };

} // namespace detray