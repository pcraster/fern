#pragma once
#include "geoneric/command/command.h"


namespace geoneric {

//! short_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED
/*!
  longer_description_HORRIBLE_LONG_STRING_TO_NOTICE_THAT_IT_SHOULD_BE_REPLACED

  \sa        .
*/
class ConvertCommand:
    public Command
{

    friend class ConvertCommandTest;

public:

                   ConvertCommand      (int argc,
                                        char** argv);

                   ~ConvertCommand     ()=default;

                   ConvertCommand      (ConvertCommand&&)=delete;

    ConvertCommand& operator=          (ConvertCommand&&)=delete;

                   ConvertCommand      (ConvertCommand const&)=delete;

    ConvertCommand& operator=          (ConvertCommand const&)=delete;

    int            execute             () const;

private:

    int            convert_to_geoneric  (int argc,
                                        char** argv) const;

    int            convert_to_cpp      (int argc,
                                        char** argv) const;

    String         convert_to_dot_ast  (ModuleVertexPtr const& tree,
                                        int modes) const;

    int            convert_to_dot_ast  (int argc,
                                        char** argv) const;

    String         convert_to_dot_flowgraph(
                                        ModuleVertexPtr const& tree) const;

    int            convert_to_dot_flowgraph(
                                        int argc,
                                        char** argv) const;

    int            convert_to_dot      (int argc,
                                        char** argv) const;

    int            convert_to_python   (int argc,
                                        char** argv) const;

    int            convert_to_xml      (int argc,
                                        char** argv) const;

};

} // namespace geoneric