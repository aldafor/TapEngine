#ifndef CRASHREPORT_H
#define CRASHREPORT_H

#include <iostream>
#include <time.h>
#include <stdarg.h>
//#include <string>
//#include <cstring>

class CrashReport
{
public:
	enum ErrorCode
	{
		NoError = 0,
		Unknown,
		NotEnoughtMemory,
		DeviceSystemError,
		GraphicDriverError,
		EngineError,
		GameError
	};

public:
	CrashReport(const char* filename = DEFAULT_FILENAME, bool enabled = true);
	virtual ~CrashReport();

	void SetError(ErrorCode code, const char* errorMessage, ...);
	const ErrorCode GetErrorCode();
	const char* GetErrorMessage();

	void ShowErrorMessage();

protected:

private:

public:

protected:

private:
	char m_filename[256];
	char m_errorMessage[2048];
	
	ErrorCode m_error;

	bool m_enabled;

	const static char* DEFAULT_FILENAME;
};

#endif //CRASHREPORT_H