#include<boost/test/unit_test.hpp>
#include<array>
#include <QPULib.h>

using namespace std;

struct vectorOpsFixture {};

const size_t VECTOR_SIZE = 16;

array<int, 16> qpu_execute( array<int, VECTOR_SIZE> matrix1,
                            array<int, VECTOR_SIZE> matrix2,
                            void (*lambda)(Ptr<Int>, Ptr<Int>, Ptr<Int>),
                            const int numOfQpusToUse = 1 ) {

    //create shared ar(ray to pass data back and forth between QPU and CPU
    SharedArray<int> qpuFirstInput{VECTOR_SIZE};
    SharedArray<int> qpuSecondInput{VECTOR_SIZE};
    SharedArray<int> qpuResult{VECTOR_SIZE};
    for(size_t index =0; index < VECTOR_SIZE; index++){
        qpuFirstInput[index] = matrix1[index];
        qpuSecondInput[index] = matrix2[index];
    }

    // init and invoke gpu
    auto kernel = compile(lambda);
    kernel.setNumQPUs(numOfQpusToUse);
    kernel(&qpuFirstInput, &qpuSecondInput, &qpuResult);

    //copy the result and return
    array<int, 16> result{};
    for(size_t index = 0; index < 16; index++) result[index] = qpuResult[index];
    return result;
}

BOOST_FIXTURE_TEST_SUITE(VectorOperationsTests, vectorOpsFixture )

    BOOST_AUTO_TEST_CASE( Add ) {
        array<int,16> m1{1, 1, 1, 1, 1, 1,
                         1, 1, 1, 1, 1, 1,
                         1, 1, 1, 1};

        array<int,16> m2{1, 1, 1, 1, 1, 1,
                         1, 1, 1, 1, 1, 1,
                         1, 1, 1, 1};

        array<int, 16> expected_result{2,2,2,2,2,
                                       2,2,2,2,2,
                                       2,2,2,2,2,
                                       2};

        auto add = [](Ptr<Int> one, Ptr<Int> two, Ptr<Int> result){ *result = *one + *two;};

        BOOST_CHECK(qpu_execute(m1, m2, add) == expected_result);
    }

BOOST_AUTO_TEST_SUITE_END()

