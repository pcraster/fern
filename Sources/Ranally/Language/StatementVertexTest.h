#ifndef INCLUDED_RANALLY_LANGUAGE_STATEMENTVERTEXTEST
#define INCLUDED_RANALLY_LANGUAGE_STATEMENTVERTEXTEST



namespace boost {
  namespace unit_test {
    class test_suite;
  }
}



class StatementVertexTest
{

public:

                   StatementVertexTest ();

  void             test                ();

  static boost::unit_test::test_suite* suite();

};

#endif
