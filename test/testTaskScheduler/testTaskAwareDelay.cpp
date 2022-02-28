#include <unityEx.h>

#define private public
#include <TaskAwareDelay.h>
#include <HardwareClock.h>
#include <RunnableInterface.h>

struct RunnableMock: public RunnableInterface
{
    int runCount = 0;
    unsigned long lastRunTime = 0;
    virtual void run(unsigned long nowMillis)
    {
        runCount++;
        lastRunTime = nowMillis;
    }
};

void testDelay()
{
    HardwareClockMock clock;    
    clock.setMicrosToStepOnRead( 500 );
    RunnableMock runnable;
    TaskAwareDelay delay ( &clock, &runnable );

    delay.delayMicros(1000);

    TEST_ASSERT_EQUAL( 1000, runnable.lastRunTime );
    TEST_ASSERT_EQUAL( 2, runnable.runCount );
    
}

void runAllTestsTaskAwareDelay()
{    
    UNITY_BEGIN_INT();
    RUN_TEST( testDelay );
    UNITY_END_INT();
}