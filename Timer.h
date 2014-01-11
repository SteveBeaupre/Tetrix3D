#ifndef CTIMER_H
#define CTIMER_H

#include <Windows.h>
//#include <MMSystem.h>

class CTimer
{
private:
	bool m_PerfCounterAvailable;
	float m_TimeScale;
	__int64 m_LastTime;          //--Holds the value when the timer is started
	__int64 m_CurrentTime;       //--Holds the current time
	__int64 m_PerfCounterFrequency;   // Performance Frequency


public:
	CTimer();
	//virtual ~CTimer();

	void  Reset();                    // Reset the timer
	void  Tick();                    //--Count how many time has pass since the last call of Tick();
	float g_ElapsedTime;        //--Holds the elapsed time (StartTime - CurrentTime)

};

#endif //--CTIMER_H