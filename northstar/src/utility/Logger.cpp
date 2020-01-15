#include "utility/Logger.hpp"
#include <stdarg.h>

northstar::utility::CLogger::CLogger(vr::IVRDriverLog* pDriverLog) {
	m_pDriverLog = pDriverLog;
}

void northstar::utility::CLogger::Log(const char* pMessage)
{
	if (m_pDriverLog)
		m_pDriverLog->Log(pMessage);
}

void northstar::utility::CLogger::LogDebug(const char* pMessage)
{
#ifdef _DEBUG
	if (m_pDriverLog)
		m_pDriverLog->Log(pMessage);
#endif
}
