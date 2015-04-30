// -----------------------------------------------------------------------------
// Fern © Geoneric
//
// This file is part of Geoneric Fern which is available under the terms of
// the GNU General Public License (GPL), version 2. If you do not want to
// be bound by the terms of the GPL, you may purchase a proprietary license
// from Geoneric (http://www.geoneric.eu/contact).
// -----------------------------------------------------------------------------
#pragma once
#include <memory>
#include "fern/language/operation/core/operations.h"


namespace fern {
namespace language {

// extern std::shared_ptr<Operations> operations;

std::shared_ptr<Operations> const& operations ();

} // namespace language
} // namespace fern
