/**
 * @author: Bilal El Uneis
 * @email: bilaleluneis@gmail.com
 * @since Nov 03 2020
 */

#include <executor.h>
#include <memory>

using IntQpuMemory = SharedArray<int>;
using FloatQpuMemory = SharedArray<float>;

IntVector execute(void (*lambda)(Ptr<Int>, Ptr<Int>, Ptr<Int>),
                  const IntVector& input1,
                  const IntVector& input2,
                  const int numOfQpusToUse) {

    IntQpuMemory qpuFirstInput{VECTOR_SIZE};
    IntQpuMemory qpuSecondInput{VECTOR_SIZE};
    IntQpuMemory qpuResult{VECTOR_SIZE};

    for(size_t index =0; index < VECTOR_SIZE; index++){
        qpuFirstInput[index] = input1[index];
        qpuSecondInput[index] = input2[index];
    }

    // init and invoke gpu
    auto kernel = compile(lambda);
    kernel.setNumQPUs(numOfQpusToUse);
    kernel(&qpuFirstInput, &qpuSecondInput, &qpuResult);

    //copy the result and return
    IntVector result{};
    for(size_t index = 0; index < 16; index++) result[index] = qpuResult[index];
    return result;
}

FloatVector execute(void (*lambda)(Ptr<Float>, Ptr<Float>, Ptr<Float>),
                    const FloatVector& input1,
                    const FloatVector& input2,
                    const int numOfQpusToUse) {

    FloatQpuMemory qpuFirstInput{VECTOR_SIZE};
    FloatQpuMemory qpuSecondInput{VECTOR_SIZE};
    FloatQpuMemory qpuResult{VECTOR_SIZE};

    for(size_t index =0; index < VECTOR_SIZE; index++){
        qpuFirstInput[index] = input1[index];
        qpuSecondInput[index] = input2[index];
    }

    // init and invoke gpu
    auto kernel = compile(lambda);
    kernel.setNumQPUs(numOfQpusToUse);
    kernel(&qpuFirstInput, &qpuSecondInput, &qpuResult);

    FloatVector result{};
    for(size_t index = 0; index < 16; index++) result[index] = qpuResult[index];
    return result;

}
