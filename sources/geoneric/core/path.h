#pragma once
#include <vector>
#include <boost/filesystem.hpp>
#include "geoneric/core/string.h"


namespace geoneric {

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        .
*/
class Path:
    private boost::filesystem::path
{

public:

                   Path                ()=default;

    // explicit       Path                (boost::filesystem::path const& path);

                   Path                (String const& string);

                   Path                (char const* string);

                   Path                (Path&&)=default;

    Path&          operator=           (Path&&)=default;

                   Path                (Path const&)=default;

    Path&          operator=           (Path const&)=default;

    // Path&          operator=           (String const& string);

                   ~Path               ()=default;

    bool           operator==          (Path const& path) const;

                   operator String     () const;

    String         generic_string      () const;

    String         native_string       () const;

    bool           is_absolute         () const;

    Path           stem                () const;

    Path           parent_path         () const;

    Path           filename            () const;

    std::vector<String> names          () const;

private:

};


std::ostream&      operator<<          (std::ostream& stream,
                                        Path const& path);

} // namespace geoneric