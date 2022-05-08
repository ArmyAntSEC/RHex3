#include <unityEx.h>
#include <SerialStream.h>

void runAllTestsLegCommandSender();
void runAllTestsLegCommandReceiver();

void process()
{
    UNITY_BEGIN_EXT();
    runAllTestsLegCommandSender();
    runAllTestsLegCommandReceiver();
    UNITY_END_EXT();
}

#ifdef ARDUINO
void setup()
{
    process();
}

void loop()
{
}
#else
int main(void)
{
    process();
}
#endif
