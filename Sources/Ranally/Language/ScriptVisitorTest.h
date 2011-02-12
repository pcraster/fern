#ifndef INCLUDED_RANALLY_LANGUAGE_SCRIPTVISITORTEST
#define INCLUDED_RANALLY_LANGUAGE_SCRIPTVISITORTEST

#include "Ranally/Language/AlgebraParser.h"
#include "Ranally/Language/XmlParser.h"
#include "Ranally/Language/ScriptVisitor.h"



namespace boost {
  namespace unit_test {
    class test_suite;
  }
}



class ScriptVisitorTest
{

private:

  ranally::language::AlgebraParser _algebraParser;
  ranally::language::XmlParser _xmlParser;
  ranally::ScriptVisitor _visitor;

public:

                   ScriptVisitorTest   ();

  void             testVisitEmptyScript();

  void             testVisitName       ();

  void             testVisitAssignment ();

  void             testVisitString     ();

  void             testVisitNumber     ();

  void             testVisitFunction   ();

  void             testVisitOperator   ();

  void             testVisitMultipleStatements();

  void             testVisitIf         ();

  void             testVisitWhile      ();

  static boost::unit_test::test_suite* suite();

};

#endif
