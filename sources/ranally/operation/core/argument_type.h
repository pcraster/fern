#pragma once
#include <iostream>


namespace ranally {

enum class ArgumentType {

    AT_ATTRIBUTE,

    AT_FEATURE

};


std::ostream&      operator<<          (std::ostream& stream,
                                        ArgumentType const& argument_type);

} // namespace ranally
