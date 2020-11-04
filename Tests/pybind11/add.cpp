/**
 * @author: Bilal El Uneis
 * @email: bilaleluneis@gmail.com
 * @since Nov 03 2020
 */

#include <boost/test/unit_test.hpp>
#include <pybind11/embed.h>

namespace py = pybind11;

struct pyInterpreterFixture {
        pyInterpreterFixture() = default;
        ~pyInterpreterFixture() = default;
        py::scoped_interpreter interpreter{};
        py::module_ qpu = py::module_::import("pyQpu");
};

BOOST_FIXTURE_TEST_SUITE(AddTests, pyInterpreterFixture )

    BOOST_AUTO_TEST_CASE( AddIntVectorsOfSize16 ) {
        py::list i;
        py::list j;
        for(int counter =0; counter< 16; counter++){
            i.append(counter);
            j.append(counter);
        }
        py::list result = qpu.attr("add")(i, j).cast<py::list>();
        BOOST_CHECK(result.size() == 16);
    }

BOOST_AUTO_TEST_SUITE_END()
