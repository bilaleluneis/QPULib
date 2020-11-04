/**
 * @author: Bilal El Uneis
 * @email: bilaleluneis@gmail.com
 * @since Nov 03 2020
 */

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <executor.h>

namespace py =  pybind11;

PYBIND11_MODULE(pyQpu, m) {

    m.def("add", [](std::vector<int> input1, std::vector<int> input2) -> std::vector<int> {
        py::gil_scoped_release nogil;
        IntVector i{};
        IntVector j{};
        std::copy_n(input1.begin(), VECTOR_SIZE, i.begin());
        std::copy_n(input2.begin(), VECTOR_SIZE, j.begin());
        IntVector r = execute([](auto i, auto j, auto r){*r = *i + *j;}, i, j);
        std::vector<int> result{};
        for(auto& element : r)
            result.push_back(element);
        return result;
    });

    m.def("add", [](std::vector<float> input1,  std::vector<float> input2) -> std::vector<float> {
        FloatVector i{};
        FloatVector j{};
        std::copy_n(input1.begin(), VECTOR_SIZE, i.begin());
        std::copy_n(input2.begin(), VECTOR_SIZE, j.begin());
        FloatVector r = execute([](auto i, auto j, auto r){*r = *i + *j;}, i, j);
        std::vector<float> result{};
        for(auto& element : r)
            result.push_back(element);
        return result;
    });

}