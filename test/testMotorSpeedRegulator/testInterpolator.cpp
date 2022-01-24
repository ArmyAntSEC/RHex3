#include <unity.h>
#include <Interpolator.h>

const unsigned int listLength = 3;
int xList[listLength] = {2, 4, 6};
int yList[listLength] = {1, 4, 9};

void testDoInterpolationOnPoint()
{
    Interpolator interpolator;

    int output = interpolator.doInterpolation( 4, xList, yList, listLength );

    TEST_ASSERT_EQUAL( 4, output );
}

void testDoInterpolationBetweenPoints()
{
    Interpolator interpolator;

    int output = interpolator.doInterpolation( 5, xList, yList, listLength );
    
    TEST_ASSERT_EQUAL( 6, output ); //Floor rounding.
}

void testDoInterpolationUnderValue()
{
    Interpolator interpolator;

    int output = interpolator.doInterpolation( 0, xList, yList, listLength );
    
    TEST_ASSERT_EQUAL( 1, output );
}

void testDoInterpolationOverValue()
{
    Interpolator interpolator;

    int output = interpolator.doInterpolation( 7, xList, yList, listLength );
    
    TEST_ASSERT_EQUAL( 9, output );
}


void processInterpolator()
{
    RUN_TEST( testDoInterpolationOnPoint );
    RUN_TEST( testDoInterpolationBetweenPoints );
    RUN_TEST( testDoInterpolationUnderValue );
    RUN_TEST( testDoInterpolationOverValue );
}