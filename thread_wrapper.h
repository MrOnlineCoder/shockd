/*

    This file defines platform-specific thread functions and includes

*/


#ifdef _WIN32 //WIN32
#include "windows.h"
#define shock_create_thread(x,y) CreateThread(NULL, NULL, x, y, NULL, NULL);

#else // LINUX

#include "pthread.h"
#define shock_create_thread(x,y) pthread_t t; pthread_create( &t, NULL, x, y);
#endif
