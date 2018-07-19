#include <sys/time.h>
#include <QPULib.h>


using namespace QPULib;

//#define USE_SCALAR_VERSION


#ifdef USE_SCALAR_VERSION
void output_pgm(int *result, int width, int height, int numIteratiosn) {
#else
void output_pgm(SharedArray<int> &result, int width, int height, int numIteratiosn) {
#endif
  FILE *fd = fopen("mandelbrot.pgm", "w") ;
  if (fd == nullptr) {
    printf("can't open file for pgm output\n");
    return;
  }

  // Write header
  fprintf(fd, "P2\n");
  fprintf(fd, "%d %d\n", width, height);
  fprintf(fd, "%d\n", numIteratiosn);

  int count; // Limit output to 10 elements per line
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      fprintf(fd, "%d ", result[x + width*y]);
      count++;
      if (count >= 10) {
        fprintf(fd, "\n");
        count = 0;
      }
    }
    fprintf(fd, "\n");
  }

  fclose(fd);
}


// ============================================================================
// Scalar version
// ============================================================================

void mandelbrot(
  float topLeftReal, float topLeftIm,
  float bottomRightReal, float bottomRightIm,
  int numStepsWidth, int numStepsHeight,
  int numiterations,
  int *result)
{
  float offsetX = (bottomRightReal - topLeftReal)/((float) numStepsWidth - 1);
  float offsetY = (topLeftIm - bottomRightIm)/((float) numStepsHeight - 1);

  for (int xStep = 0; xStep < numStepsWidth; xStep++) {
    for (int yStep = 0; yStep < numStepsHeight; yStep++) {
      float realC = topLeftReal   + ((float) xStep)*offsetX;
      float imC   = bottomRightIm + ((float) yStep)*offsetY;

      int count = 0;
      float real = realC;
      float im   = imC;
      float radius = (real*real + im*im);
      while (radius < 4 && count < numiterations) {
        float tmpReal = real*real - im*im;
        float tmpIm   = 2*real*im;
        real = tmpReal + realC;
        im   = tmpIm + imC;

        radius = (real*real + im*im);
        count++;
      }

      result[xStep + yStep*numStepsWidth] = count;
    }
  }
}


// ============================================================================
// Vector version
// ============================================================================

void mandelbrotCore(
  Float reC, Float imC,
  Int resultIndex,
  Int numiterations,
  Ptr<Int> &result)
{
  Float re = reC;
  Float im = imC;
  Int count = 0;

  Float reSquare = re*re;
  Float imSquare = im*im;

  // Following is a float version of boolean expression: ((reSquare + imSquare) < 4 && count < numiterations)
  // It works because `count` increments monotonically.
  FloatExpr condition = (4.0f - (reSquare + imSquare))*toFloat(numiterations - count);
  Float checkvar = condition;

  While (any(checkvar > 0))
    Where (checkvar > 0)
      Float imTmp = 2*re*im;
      re   = (reSquare - imSquare) + reC;
      im   = imTmp  + imC;

      reSquare = re*re;
      imSquare = im*im;
      count++;

      checkvar = condition; 
    End
  End

  store(count, result + resultIndex);
  //result[resultIndex] = count;
}


void mandelbrot_1(
  Float topLeftReal, Float topLeftIm,
  Float offsetX, Float offsetY,
  Int numStepsWidth, Int numStepsHeight,
  Int numiterations,
  Ptr<Int> result)
{
  Float imC = topLeftIm;

  For (Int yStep = 0, yStep < numStepsHeight, yStep++)
    Float reLine = topLeftReal;

    For (Int xStep = 0, xStep < numStepsWidth, xStep = xStep + 16)
      Float reC = reLine + offsetX*toFloat(index());

      mandelbrotCore(
        reC, imC,
        (xStep + index() + yStep*numStepsWidth),
        numiterations,
        result);

      reLine = reLine + 16.0f*offsetX;
    End

    imC = imC - offsetY;
  End
}


/**
 * @brief Multi-QPU version
 */
void mandelbrot_2(
  Float topLeftReal, Float topLeftIm,
  Float offsetX, Float offsetY,
  Int numStepsWidth, Int numStepsHeight,
  Int numiterations,
  Ptr<Int> result)
{
  Int inc = numQPUs();

  For (Int yStep = 0, yStep < numStepsHeight, yStep = yStep + inc)
    Int yIndex = me() + yStep;

    For (Int xStep = 0, xStep < numStepsWidth, xStep = xStep + 16)
      Int xIndex = index() + xStep;
      Int resultIndex = xIndex + yIndex*numStepsWidth;

      For (Int dummy = 0, dummy < 1 && (resultIndex < (numStepsWidth*numStepsHeight)), dummy++)
      //Where (resultIndex < (numStepsWidth*numStepsHeight))  // Only calculate if we're within bounds
        mandelbrotCore(
          (topLeftReal + offsetX*toFloat(xIndex)),
          (topLeftIm - toFloat(yIndex)*offsetY),
          resultIndex,
          numiterations,
          result);
      End
    End
  End
}

// ============================================================================
// Main
// ============================================================================

int main()
{
  // Timestamps
  timeval tvStart, tvEnd, tvDiff;

  // Initialize constants for the kernels
  const int numStepsWidth  = 1024;
  const int numStepsHeight = 1024;
  const int numiterations  = 1024;
  const float topLeftReal = -2.5f;
  const float topLeftIm =  2.0f;
  const float bottomRightReal = 1.5f;
  const float bottomRightIm = -2.0f;


#ifdef USE_SCALAR_VERSION
  // Allocate and initialise
  int *result = new int [numStepsWidth*numStepsHeight];
#else
  const int NUM_ITEMS = numStepsWidth*numStepsHeight;
  SharedArray<int> result(NUM_ITEMS);

  // Construct kernel
  auto k = compile(mandelbrot_2);
  k.setNumQPUs(12);

#endif

  gettimeofday(&tvStart, NULL);
#ifdef USE_SCALAR_VERSION
  mandelbrot(topLeftReal, topLeftIm,bottomRightReal, bottomRightIm, numStepsWidth, numStepsHeight, numiterations, result);
#else
  assert(0 == numStepsWidth % 16);  // width needs to be a multiple of 16
  float offsetX = (bottomRightReal - topLeftReal)/((float) numStepsWidth - 1);
  float offsetY = (topLeftIm - bottomRightIm)/((float) numStepsHeight - 1);

  k(
    topLeftReal, topLeftIm,
    offsetX, offsetY,
    numStepsWidth, numStepsHeight,
    numiterations,
    &result);

#endif

  gettimeofday(&tvEnd, NULL);
  timersub(&tvEnd, &tvStart, &tvDiff);

  printf("%ld.%06lds\n", tvDiff.tv_sec, tvDiff.tv_usec);
  output_pgm(result, numStepsWidth, numStepsHeight, numiterations);

  return 0;
}
