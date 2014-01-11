#include "Timer.h"

CTimer::CTimer()
{
	//Check if a High resolution timer is available 
	if (QueryPerformanceFrequency((LARGE_INTEGER *)&m_PerfCounterFrequency)) 
    { 
		m_PerfCounterAvailable = true;
		QueryPerformanceCounter((LARGE_INTEGER *) &m_CurrentTime); 
		m_TimeScale			   = 1.0f / m_PerfCounterFrequency;
	} 
    else 
    { 
		// no performance counter, read in using timeGetTime 
		m_PerfCounterAvailable = false;
		m_CurrentTime		   = timeGetTime(); 
		m_TimeScale			   = 0.001f;
	
    } 
	
	Reset();
}


void CTimer::Reset()
{
    // Is performance hardware available?
	if(m_PerfCounterAvailable){
        // Query high-resolution performance hardware
		QueryPerformanceCounter((LARGE_INTEGER *)&m_CurrentTime);
	} else  {
        // Fall back to less accurate timer
		m_CurrentTime = timeGetTime();

	} // End If no hardware available

	// Init m_LastTime and g_ElapsedTime
	m_LastTime = m_CurrentTime;
	g_ElapsedTime = 0.0f;
}


void CTimer::Tick()
{
	// This will be used to compare how many time has pass since the last Tick()
	m_LastTime = m_CurrentTime;

    // Is performance hardware available?
	if(m_PerfCounterAvailable){
        // Query high-resolution performance hardware
		QueryPerformanceCounter((LARGE_INTEGER *)&m_CurrentTime);
	} else  {
        // Fall back to less accurate timer
		m_CurrentTime = timeGetTime();

	} // End If no hardware available

	// Calculate the elapsed time
	g_ElapsedTime = (m_CurrentTime - m_LastTime) * m_TimeScale;
}

