#include "CrashReport.h"

const char* CrashReport::DEFAULT_FILENAME = "CrashReport.txt";

CrashReport::CrashReport(const char* filename /*= DEFAULT_FILENAME*/, bool enabled /*= true*/ )
{
	m_enabled = enabled;

	// Initialize report state without error
	SetError(NoError, "No Error");

	// Don't generate any crash report file if is disabled
	if(!enabled)
		return;

	// Use default filename
	if(filename == NULL)
	{
		if(strlen(m_filename) >= strlen(DEFAULT_FILENAME))
			sprintf(m_filename, DEFAULT_FILENAME);
	}
	else
	{
		// Check if we need to add the extension
		if( (strlen(filename) > 4) && (filename[strlen(filename) - 4] == '.') )
		{
			if(strlen(m_filename) >= strlen(filename))
				sprintf(m_filename, "%s", filename);
		}
		else
		{
			if(strlen(m_filename) >= strlen(filename) + 4)
				sprintf(m_filename, "%s.txt", filename);
		}
	}

	//Create a new log file
	FILE *file;
	file = fopen(m_filename, "wt");
	if(file != NULL)
	{
		char timestampBuffer[128];

		//Display time and date
		_strdate(timestampBuffer);
		fprintf(file, "Date(M/D/Y): %s ", timestampBuffer);
		_strtime(timestampBuffer);
		fprintf(file, "%s\n\n", timestampBuffer);
		
		fclose(file);
	}
}

CrashReport::~CrashReport()
{
	// Leave virtual destructor for a possible 
}

void CrashReport::SetError(CrashReport::ErrorCode code, const char* errorMessage, ...)
{
	// Set error message
	va_list argptr;
	va_start(argptr, errorMessage);
	vsprintf(m_errorMessage, errorMessage, argptr);
	va_end(argptr);

	// Set error code
	m_error = code;

	// If disabled don't write to crash report file
	if(!m_enabled)
		return;

	// Open crash report file and write the error
	FILE *file;
	file = fopen(m_filename, "at");
	if(file != NULL)
	{
		fprintf(file, "%s\n", m_errorMessage);
		fclose(file);
	}
}

const CrashReport::ErrorCode CrashReport::GetErrorCode()
{
	return m_error;
}

const char* CrashReport::GetErrorMessage()
{
	return m_errorMessage;
}

void CrashReport::ShowErrorMessage()
{
	/*switch(m_EC_ID)
	{
	case EC_NoError:
	{
	MessageBox( GetDesktopWindow(),
	m_szErrorMsg,
	"All Okay",
	MB_ICONINFORMATION | MB_OK);
	break;
	}

	case EC_Windows:
	{
	MessageBox( GetDesktopWindow(),
	m_szErrorMsg,
	"Windows Error",
	MB_ICONERROR | MB_OK);
	break;
	}

	case EC_Unknown:
	case EC_Error:
	{
	MessageBox( GetDesktopWindow(),
	m_szErrorMsg,
	"Error",
	MB_ICONERROR | MB_OK);
	break;
	}

	case EC_NotEnoughtMemory:
	{
	MessageBox( GetDesktopWindow(),
	m_szErrorMsg,
	"Memory Error",
	MB_ICONERROR | MB_OK);
	break;
	}

	case EC_DirectX:
	{
	MessageBox( GetDesktopWindow(),
	m_szErrorMsg,
	"DirectX9 Error",
	MB_ICONERROR | MB_OK);
	break;
	}

	case EC_OpenGL:
	{
	MessageBox( GetDesktopWindow(),
	m_szErrorMsg,
	"OpenGL Error",
	MB_ICONERROR | MB_OK);
	break;
	}

	case EC_GameError:
	{
	MessageBox( GetDesktopWindow(),
	m_szErrorMsg,
	"Game Error",
	MB_ICONERROR | MB_OK);
	break;
	}

	default:
	{
	MessageBox( GetDesktopWindow(),
	"Unrecognized error message",
	"Unknown Message",
	MB_ICONQUESTION | MB_OK);
	break;
	}
	}*/
}
