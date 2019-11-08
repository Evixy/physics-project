#ifdef WINDOWS
#define ALIGN16 __declspec(align(16))
#elif defined LINUX
#define ALIGN16 __attribute__ ((aligned(16)))
#endif

#define PI 3.14159265f