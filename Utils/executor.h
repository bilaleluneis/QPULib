/**
 * @author: Bilal El Uneis
 * @email: bilaleluneis@gmail.com
 * @since Nov 03 2020
 */

#ifndef QPULIB_EXECUTOR_H
#define QPULIB_EXECUTOR_H

#include <array>
#include <QPULib.h>

const size_t VECTOR_SIZE = 16;

using IntVector = std:: array<int, VECTOR_SIZE>;
using FloatVector = std:: array<float, VECTOR_SIZE>;

IntVector execute(void (*lambda)(Ptr<Int>, Ptr<Int>, Ptr<Int>),
                  const IntVector& input1,
                  const IntVector& input2,
                  const int numOfQpusToUse = 1 );

FloatVector execute(void (*lambda)(Ptr<Float>, Ptr<Float>, Ptr<Float>),
                    const FloatVector& input1,
                    const FloatVector& input2,
                    const int numOfQpusToUse = 1 );

#endif //QPULIB_EXECUTOR_H
