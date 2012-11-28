#include "ranally/core/messages.h"


namespace ranally {

Messages::Messages()
{
    insert(std::make_pair(MessageId::UNKNOWN_ERROR,
        "Sorry, unknown error"));
    insert(std::make_pair(MessageId::ERROR_PARSING,
        "Error while parsing: %1%"));
    insert(std::make_pair(MessageId::ERROR_PARSING_FILE,
        "Error while parsing file %1%: %2%"));
}


String const& Messages::operator[](
    MessageId message_id) const
{
    const_iterator it = find(message_id);
    // If this fails, the message_id hasn't been added to the collection. See
    // above.
    assert(it != end());

    if(it == end()) {
        // Pick a default error message. Just don't crash.
        it = find(MessageId::UNKNOWN_ERROR);
    }
    assert(it != end());

    return (*it).second;
}

} // namespace ranally