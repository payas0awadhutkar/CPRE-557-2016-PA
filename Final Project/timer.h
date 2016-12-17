#include "math.h"
#include "mmsystem.h"

struct {
  __int64       frequency;          // Timer Frequency
  GLdouble            resolution;          // Timer Resolution
  unsigned long mm_timer_start;     
  
  // Multimedia Timer Start Value
  unsigned long mm_timer_elapsed;      // Multimedia Timer Elapsed Time
  bool   performance_timer;    
  
  // Using The Performance Timer?
  __int64       performance_timer_start;      // Performance Timer Start Value
  __int64       performance_timer_elapsed; // Performance Timer Elapsed Time
} timer;

// Initialize Our Timer
void TimerInit(void) {
     memset(&timer, 0, sizeof(timer));   
 // Clear Our Timer Structure
     // Check To See If A Performance Counter Is Available
     // If One Is Available The Timer Frequency Will Be Updated
     if (!QueryPerformanceFrequency((LARGE_INTEGER *) &timer.frequency))
     {
          // No Performace Counter Available
          timer.performance_timer = FALSE;                      // Set Performance Timer To FALSE
          timer.mm_timer_start = timeGetTime();                 // Use timeGetTime()
          timer.resolution  = 1.0f/1000.0f;                           // Set Our Timer Resolution To .001f
          timer.frequency   = 1000;                                     // Set Our Timer Frequency To 1000
          timer.mm_timer_elapsed = timer.mm_timer_start; // Set The Elapsed Time
     }
     else
     {
          // Performance Counter Is Available, Use It Instead Of The Multimedia Timer
          // Get The Current Time And Store It In performance_timer_start
          QueryPerformanceCounter((LARGE_INTEGER *) &timer.performance_timer_start);
          timer.performance_timer   = TRUE;    // Set Performance Timer To TRUE
          // Calculate The Timer Resolution Using The Timer Frequency
          timer.resolution    = (GLdouble) (((double)1.0f)/((double)timer.frequency));
          // Set The Elapsed Time To The Current Time
          timer.performance_timer_elapsed = timer.performance_timer_start;
     }
}
// Get Time In Milliseconds
inline GLdouble TimerGetTime()
{
  __int64 time;                                  // 'time' Will Hold A 64 Bit Integer
  if (timer.performance_timer)           // Are We Using The Performance Timer?
  {
    QueryPerformanceCounter((LARGE_INTEGER *) &time); // Current Performance Time
    // Return The Time Elapsed since TimerInit was called
    return ( (GLdouble) ( time - timer.performance_timer_start) * timer.resolution)*1000.0f;
  }
  else
  {
    // Return The Time Elapsed since TimerInit was called
    return ( (GLdouble) ( timeGetTime() - timer.mm_timer_start) * timer.resolution)*1000.0f;
  }
}

GLdouble Time1;
GLdouble Time2;
GLdouble DiffTime;
GLdouble StartTime;
