#pragma once
#include "geoneric/core/script_error.h"


namespace geoneric {

class ParseError:
    public ScriptError
{
public:

                   ParseError          (String const& source_name,
                                        long line_nr,
                                        long col_nr,
                                        String const& message);

                   ParseError          (String const& source_name,
                                        long line_nr,
                                        long col_nr,
                                        String statement,
                                        String const& message);

                   ParseError          (ParseError const&)=default;

    ParseError&    operator=           (ParseError const&)=default;

                   ParseError          (ParseError&&)=default;

    ParseError&    operator=           (ParseError&&)=default;

                   ~ParseError         ()=default;

    String         message             () const;

private:

    String         _statement;

    String         _message;

};

} // namespace geoneric