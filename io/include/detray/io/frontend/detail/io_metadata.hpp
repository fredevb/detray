/** Detray library, part of the ACTS project (R&D line)
 *
 * (c) 2022-2024 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#pragma once

// Project include(s)
#include "detray/utils/type_list.hpp"
#include "detray/version.hpp"  // generated by cmake

// System include(s).
#include <ctime>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <string_view>

namespace detray::io::detail {

template <typename detector_t>
inline void print_type_warning(const std::string& subject) {

    using detector_list_t = detray::types::list<typename detector_t::metadata>;

    std::cout << "WARNING: Detector of type ";
    types::print<detector_list_t>();
    std::cout << "does not support " << subject << std::endl;
}

/// @returns a string that contains the current date and time
/// @see https://en.cppreference.com/w/cpp/chrono/c/strftime
inline std::string get_current_date() {
    std::time_t time = std::time({});
    char timeString[std::size("yyyy-mm-ddT hh:mm:ssZ")];
    std::strftime(std::data(timeString), std::size(timeString), "%FT %TZ",
                  std::gmtime(&time));
    return timeString;
}

/// @returns a string that contains the current project version
inline std::string get_detray_version() {
    return "detray - " + std::string(detray::version);
}

static constexpr std::string_view minimal_io_version = "detray - 0.52.0";

}  // namespace detray::io::detail
