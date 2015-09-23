#ifndef CTIMER_H
#define CTIMER_H

/**
@author Tom Krajnik
@basic timer class 
*/
#include <sys/time.h>
#include <stdlib.h>

#define TIMEOUT_INTERVAL 40000

class CTimer
{
	public:
		CTimer(int timeOut = TIMEOUT_INTERVAL);
		~CTimer();

		/*time in microseconds*/
		int getTime();

		/*timer reset,start,pause*/
		void reset(int timeOut = TIMEOUT_INTERVAL);
		int pause();
		int start();

		/*was there a timeout ?*/
		bool timeOut();

		/*is the timer running ?*/
		bool paused();
		int64_t getRealTime();
	private:
		int startTime;
		int pauseTime;
		bool running;
		int timeoutInterval;
};

#endif
