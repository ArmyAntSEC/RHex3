#include <unity.h>

#ifdef ARDUINO
#define UNITY_BEGIN_EXT() UNITY_BEGIN() 
#define UNITY_END_EXT() UNITY_END() 
#define UNITY_BEGIN_INT() 
#define UNITY_END_INT() 
#else
#define UNITY_BEGIN_EXT()
#define UNITY_END_EXT()
#define UNITY_BEGIN_INT() UNITY_BEGIN() 
#define UNITY_END_INT() UNITY_END() 
#endif