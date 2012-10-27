/**
 * To run the program:
 *     g++ -lcppunit -ldl -Wall TestSSSP.c++ -o TestSSSP.app
 */
// --------
// includes
// --------
#include "cppunit/extensions/HelperMacros.h" // CPPUNIT_TEST, CPPUNIT_TEST_SUITE, CPPUNIT_TEST_SUITE_END
#include "cppunit/TestFixture.h"             // TestFixture
#include "cppunit/TextTestRunner.h"          // TestRunner
#include "SSSP.h"

struct TestSSSP : CppUnit::TestFixture {
    void test_read_graph() {
    }
    void test_dijkstra() {
    }
    void test_bellman_ford() {
    }
    void test_roundbase_relaxation() {
    }

// -----
// suite
// -----

CPPUNIT_TEST_SUITE(TestSSSP);
        CPPUNIT_TEST(test_read_graph);
        CPPUNIT_TEST(test_dijkstra);
        CPPUNIT_TEST(test_bellman_ford);
        CPPUNIT_TEST(test_roundbase_relaxation);
    CPPUNIT_TEST_SUITE_END();
};

// ----
// main
// ----

int main() {
    using namespace std;
    ios_base::sync_with_stdio(false);    // turn off synchronization with C I/O
    cout << "TestSSSP.c++" << endl;

    CppUnit::TextTestRunner tr;

    tr.addTest(TestSSSP::suite());
    tr.run();

    cout << "Done." << endl;
    return 0;
}
