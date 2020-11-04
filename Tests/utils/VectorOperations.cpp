/**
 * @author: Bilal El Uneis
 * @email: bilaleluneis@gmail.com
 * @since Nov 03 2020
 */

#include <boost/test/unit_test.hpp>
#include <executor.h>


struct vectorOpsFixture {};



BOOST_FIXTURE_TEST_SUITE(VectorOperationsTests, vectorOpsFixture )

    BOOST_AUTO_TEST_CASE( Add ) {
        IntVector m1{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

        IntVector m2{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

        IntVector expected_result{2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

        BOOST_CHECK(execute([](auto i, auto j, auto r){*r = *i + *j;}, m1, m2) == expected_result);
    }

BOOST_AUTO_TEST_SUITE_END()

