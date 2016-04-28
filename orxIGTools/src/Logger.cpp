#include "orxIGToolsPCH.h"
#include "Logger.h"

#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <cstring>

#include <iostream>

#ifdef WIN32
#include <windows.h>
#endif

#if defined(linux)
#include <sys/socket.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#endif

namespace orxSmith
	{
	namespace Core
		{


// 		IMPLEMENT_SINGLETON(Logger);

		// COMMON DEFINITIONS
#ifdef LOG_USE_UNICODE

#define FOPEN _wfopen
#define STRCAT wcscat
#define STRLEN wcslen
#define SPRINTF swprintf
#define VSPRINTF vswprintf
#define GET_MODULE_FILENAME GetModuleFileNameW
#define OUTPUT_DEBUG_STRING OutputDebugStringW

#else

#define FOPEN fopen
#define STRCAT strcat
#define STRLEN strlen
#define SPRINTF sprintf
#define VSPRINTF vsprintf
#define GET_MODULE_FILENAME GetModuleFileNameA
#define OUTPUT_DEBUG_STRING OutputDebugStringA

#endif



#if defined (linux)
		static pthread_mutex_t g_LogCS = PTHREAD_MUTEX_INITIALIZER;
#define ENTER_CRITICAL_SECTION		pthread_mutex_lock
#define LEAVE_CRITICAL_SECTION		pthread_mutex_unlock
#define GET_PID()                   (int)getpid()
#else
		static CRITICAL_SECTION	g_LogCS;
#define ENTER_CRITICAL_SECTION		EnterCriticalSection
#define LEAVE_CRITICAL_SECTION		LeaveCriticalSection
#define GET_PID()                   GetCurrentThreadId()
#endif


		/******************************************************************************
		Function :			Logger
		******************************************************************************/
		Logger::Logger(log_string filename, bool use_debug, bool use_date, bool use_file, bool use_cout)
			{
			m_pFile = NULL;
			m_bInitialized = false;
			m_szFilename = filename;
			m_bUseDate = use_date;
			m_bEnableDbgOut = use_debug;
			m_bUseFileOut = use_file;
			m_bEnableCout = use_cout;

			m_Level = Level::All;

			m_pCustomCallback = NULL;
			m_bSocketDbgOut = false;

			initLogger(filename, m_Level);
			}

		/******************************************************************************
		Function :			Logger
		******************************************************************************/
		Logger::~Logger(void)
			{
			close();
#if defined (WIN32)
			DeleteCriticalSection(&g_LogCS);
#endif
			}

		/******************************************************************************
		Function :			getDefaultFilename

		Description	: Returns the name of the current module
		******************************************************************************/
		log_string Logger::getDefaultFilename()
			{
			return log_string(LOG_TEXT(""));
			}


		/*******************************************************************************
		InitLogger()

		Description : Initialize logger
		Parameters  : Name of the file to use for logging
		: boolean to say you want to include timestamp
		: boolean to say you want to include debug output
		: int to determine the starting level
		Returns     : true if that's all right, false in case we canìt open the file
		*******************************************************************************/
		bool Logger::initLogger(log_string szFilename, Level level)
			{
			if (szFilename.length() == 0)
				szFilename = getDefaultFilename();

			if (m_pFile != NULL)
				fclose(m_pFile);

			m_Level = m_Level;
			m_szFilename = szFilename;

#if defined (WIN32)
			::InitializeCriticalSection(&g_LogCS);
#endif

			m_bInitialized = true;
			if (m_bUseFileOut)
				m_bInitialized = InitializeFile(m_szFilename);
			return m_bInitialized;
			}


		//+----------------------------------------------------------------------+
		bool Logger::InitializeFile(log_string filename)
			{
			log_string path = filename;
			if (path.empty())
				{
				log_string execPath;
				log_string rootPath;
				/// Find parent of executable directory
#if defined(linux)
				path = LOG_TEXT("/var/log/");
				path += m_szFilename;
#else
				char szPath[MAX_PATH];
				if (GET_MODULE_FILENAME(NULL, szPath, MAX_PATH))
					{
					path = szPath;
					size_t last = path.rfind('\\');
					if (last != log_string::npos)
						rootPath = path.substr(0, last + 1);
					}
				path = rootPath + LOG_TEXT("\\") + m_szFilename;
#endif
				}

			// create a clean file
			m_pFile = FOPEN(path.c_str(), LOG_TEXT("w+"));
			if (m_pFile)
				fclose(m_pFile);
			else
				{
#if defined(linux)
				printf("Failed to create log file : %ld", (intptr_t)m_pFile);
#else
				printf("Failed to create log file : %ld", GetLastError());
#endif
				return false;
				}

			return true;
			}

		/******************************************************************************
		Function :                      GetTime

		Description   : Prints the current time into the given buffer (at least 20 chars)
		Returns       : none
		******************************************************************************/
		void Logger::GetTime(char * szBuffer, int iBufLen)
			{
			if (iBufLen < 20)
				return;

#if defined (linux)
			timer_t tim = (timer_t)time(NULL);
			tm * now = localtime((time_t*)&tim);
			SPRINTF(szBuffer, LOG_TEXT("%02d/%02d/%02d-%02d:%02d:%02d "),
				now->tm_wday,
				now->tm_mon,
				now->tm_year,
				now->tm_hour,
				now->tm_min,
				now->tm_sec);
#else
			SYSTEMTIME stime;
			GetLocalTime(&stime);
			SPRINTF(szBuffer, LOG_TEXT("%02d/%02d/%02d-%02d:%02d:%02d.%03d "),
				stime.wDay,
				stime.wMonth,
				stime.wYear,
				stime.wHour,
				stime.wMinute,
				stime.wSecond,
				stime.wMilliseconds);
#endif
			}

		/******************************************************************************
		Function :                      trace

		Description   : Traces contents at a given level
		Parameters    : level         - level of current trace
		szFormat      - format of the trace (see printf format)
		...           - parameters to trace
		Returns               : none
		******************************************************************************/
		void Logger::Log(Level level, const char * szFormat, ...)
			{
			va_list arg;
			WORD  usLength;
			char szBuffer[LOGGER_DEFAULT_BUFFER];
			log_string szLev;

			/////////////////////////////////////////////////////
			ENTER_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////

			if (m_bInitialized && (level <= m_Level))
				{
				memset(szBuffer, 0, LOGGER_DEFAULT_BUFFER);
				if (m_bUseDate)
					GetTime(szBuffer, LOGGER_DEFAULT_BUFFER);

				if (m_bUseThreadID)
					{
					DWORD tid = GET_PID();
					SPRINTF((szBuffer + STRLEN(szBuffer)), LOG_TEXT(":: PID=0x%X "), (unsigned int)tid);
					}

				getLevelString(level, &szLev);
				STRCAT(szBuffer, szLev.c_str());
				usLength = (WORD)STRLEN(szBuffer);
				va_start(arg, szFormat);
				VSPRINTF((szBuffer + usLength), szFormat, arg);
				STRCAT(szBuffer, LOG_TEXT("\r\n"));
				PrintOut(szBuffer, (WORD)STRLEN(szBuffer) * sizeof(char));
				}

			/////////////////////////////////////////////////////
			LEAVE_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			}

		/******************************************************************************
		Function :                      trace2

		Description   : Traces contents ignoring level
		Parameters    : level         - level of current trace
		szFormat      - format of the trace (see printf format)
		...           - parameters to trace
		Returns               : none
		******************************************************************************/
		void Logger::Log2(const char * szFormat, ...)
			{
			va_list arg;
			char szBuffer[LOGGER_DEFAULT_BUFFER];

			/////////////////////////////////////////////////////
			ENTER_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			if (m_bInitialized)
				{
				va_start(arg, szFormat);
				VSPRINTF(szBuffer, szFormat, arg);
				STRCAT(szBuffer, LOG_TEXT("\r\n"));
				PrintOut(szBuffer, (WORD)STRLEN(szBuffer) * sizeof(char));
				}
			/////////////////////////////////////////////////////
			LEAVE_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			}


		/*******************************************************************************
		LogDbg()

		Description : Main logging function.
		Parameters  : format to use (see printf documentation) and parameter to trace
		Returns     : nothing
		*******************************************************************************/
		void Logger::LogDbg(log_string file, int line, const char * pszFormat, ...)
			{
			char szBuffer[LOGGER_DEFAULT_BUFFER];
			char szline[10];

			/////////////////////////////////////////////////////
			ENTER_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			va_list args;
			if (m_bInitialized)
				{
				va_start(args, pszFormat);
				VSPRINTF(szBuffer, pszFormat, args);
				STRCAT(szBuffer, LOG_TEXT("\r\n"));
				STRCAT(szBuffer, file.c_str());
				SPRINTF((LPSTR)szline, LOG_TEXT(" (%d)"), line);
				STRCAT(szBuffer, szline);
				PrintOut(szBuffer, (WORD)STRLEN(szBuffer) * sizeof(char));
				}
			/////////////////////////////////////////////////////
			LEAVE_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			}


		/******************************************************************************
		Function : LogBuffer

		Description	: Logs a buffer in hexadecimal format
		Parameters  : buffer to log and buffer length
		Returns     : nothing
		******************************************************************************/
		void Logger::LogBuffer(Level level, const char *Text, const void * buffer, int buf_len)
			{
			assert(buffer != 0);

			char szBuffer[LOGGER_DEFAULT_BUFFER];
			log_string szLev;
			vector<log_string> output;

			/////////////////////////////////////////////////////
			ENTER_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////

			if (m_bInitialized && (level <= m_Level))
				{
				memset(szBuffer, 0, LOGGER_DEFAULT_BUFFER);
				if (m_bUseDate)
					GetTime(szBuffer, LOGGER_DEFAULT_BUFFER);

				if (m_bUseThreadID)
					{
					DWORD tid = GET_PID();
					SPRINTF(&szBuffer[STRLEN(szBuffer)], LOG_TEXT(":: PID=0x%X "), (unsigned int)tid);
					}

				getLevelString(level, &szLev);
				STRCAT(szBuffer, szLev.c_str());
				STRCAT(szBuffer, Text);
				SPRINTF(&szBuffer[STRLEN(szBuffer)], LOG_TEXT(" size = %d"), buf_len);
				STRCAT(szBuffer, LOG_TEXT("\n"));
				PrintOut(szBuffer, (WORD)STRLEN(szBuffer) * sizeof(char));

				output = get_buffer_lines(buffer, buf_len);
				for (size_t a = 0; a<output.size(); a++)
					PrintOut((char *)(output[a].c_str()), output[a].length());

				}
			/////////////////////////////////////////////////////
			LEAVE_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			}

		void Logger::LogBuffer(Level level, const char *Text, log_string buffer)
			{
			char szBuffer[LOGGER_DEFAULT_BUFFER];
			log_string szLev;
			vector<log_string> output;

			/////////////////////////////////////////////////////
			ENTER_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////

			if (m_bInitialized && (level <= m_Level))
				{
				memset(szBuffer, 0, LOGGER_DEFAULT_BUFFER);
				if (m_bUseDate)
					GetTime(szBuffer, LOGGER_DEFAULT_BUFFER);

				if (m_bUseThreadID)
					{
					DWORD tid = GET_PID();
					SPRINTF(&szBuffer[STRLEN(szBuffer)], LOG_TEXT(":: PID=0x%X "), (unsigned int)tid);
					}

				getLevelString(level, &szLev);
				STRCAT(szBuffer, szLev.c_str());
				STRCAT(szBuffer, Text);
				SPRINTF(&szBuffer[STRLEN(szBuffer)], LOG_TEXT(" size = %d"), (int)buffer.size());
				STRCAT(szBuffer, LOG_TEXT("\n"));
				PrintOut(szBuffer, (WORD)STRLEN(szBuffer) * sizeof(char));

				output = get_buffer_lines(buffer);
				for (size_t a = 0; a<output.size(); a++)
					PrintOut((char *)(output[a].c_str()), output[a].length());

				}
			/////////////////////////////////////////////////////
			LEAVE_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////

			}



		/******************************************************************************
		Function :                      get_buffer_lines

		Description   : Hex view routine to get vector of string with hex view
		******************************************************************************/
		vector<log_string> Logger::get_buffer_lines(const void * buffer, int buf_len)
			{
			int lines = buf_len / BYTE_PER_LINE;
			if (buf_len < BYTE_PER_LINE)
				lines = 1;

			vector<log_string> output;
			log_string bf = LOG_TEXT("Buffer = ");
			log_string bfb;
			char bt[6];

			if (buffer != NULL)
				{
				try
					{
					int counter = 0;
					for (int line = 0; line<lines; line++)
						{
						log_string ln = LOG_TEXT("");
						for (int cnt = 0; cnt < BYTE_PER_LINE; cnt++)
							{
							if (counter < buf_len)
								{
								BYTE b = *((unsigned char*)buffer + counter);
								SPRINTF(bt, LOG_TEXT("%.2X "), b);
								ln += bt;
								if ((b > 0x1e) && (b <= 0xfe))
									bfb += b;
								else
									bfb += LOG_TEXT(".");
								}
							else
								break;
							counter++;
							}
						ln += LOG_TEXT("    ") + bfb;
						ln += LOG_TEXT("\n");
						bfb = LOG_TEXT("");
						output.push_back(ln);
						}
					}
				catch (...)
					{

					}
				}
			return output;
			}

		/******************************************************************************
		Function :                      get_buffer_lines

		Description   : Hex view routine to get vector of string with hex view
		******************************************************************************/
		vector<log_string> Logger::get_buffer_lines(log_string buffer)
			{
			int buf_len = buffer.size();
			int lines = buf_len / BYTE_PER_LINE;
			if (buf_len < BYTE_PER_LINE)
				lines = 1;

			vector<log_string> output;
			log_string bf = LOG_TEXT("Buffer = ");
			log_string bfb;
			char bt[6];

			try
				{
				int counter = 0;
				for (int line = 0; line<lines; line++)
					{
					log_string ln = LOG_TEXT("");
					for (int cnt = 0; cnt < BYTE_PER_LINE; cnt++)
						{
						if (counter < buf_len)
							{
							BYTE b = buffer[counter];
							SPRINTF(bt, LOG_TEXT("%.2X "), b);
							ln += bt;
							if ((b > 0x1e) && (b <= 0xfe))
								bfb += b;
							else
								bfb += LOG_TEXT(".");
							}
						else
							break;
						counter++;
						}
					ln += LOG_TEXT("    ") + bfb;
					ln += LOG_TEXT("\n");
					bfb = LOG_TEXT("");
					output.push_back(ln);
					}
				}
			catch (...)
				{

				}
			return output;
			}

		/******************************************************************************
		Function :                      setLevel

		Description   : Changes the trace maximum leve
		Parameters    : level         - levet to set
		Returns               : none
		******************************************************************************/
		void Logger::setLevel(int level)
			{
			/////////////////////////////////////////////////////
			ENTER_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			m_Level = (Level)level;
			/////////////////////////////////////////////////////
			LEAVE_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			if (m_bInitialized)
				Log(m_Level, LOG_TEXT("Changing maximum level to %d"), m_Level);
			}

		/******************************************************************************
		Function :                      setUseDate

		Description   : Enables/disables date/time prints
		Parameters    : bUseDate      - true enables, false disables
		Returns               : none
		******************************************************************************/
		void Logger::setUseFile(bool bUseFile, log_string filename)
			{
			/////////////////////////////////////////////////////
			ENTER_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			m_bUseFileOut = bUseFile;
			m_szFilename = filename;
			InitializeFile(m_szFilename);
			/////////////////////////////////////////////////////
			LEAVE_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			if (m_bInitialized)
				Log(m_Level, LOG_TEXT("%s file output"), (m_bUseFileOut) ? (LOG_TEXT("Enabling")) : (LOG_TEXT("Disabling")));
			}

		/******************************************************************************
		Function :                      setUseDate

		Description   : Enables/disables date/time prints
		Parameters    : bUseDate      - true enables, false disables
		Returns               : none
		******************************************************************************/
		void Logger::setUseDate(bool bUseDate)
			{
			/////////////////////////////////////////////////////
			ENTER_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			m_bUseDate = bUseDate;
			/////////////////////////////////////////////////////
			LEAVE_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			if (m_bInitialized)
				Log(m_Level, LOG_TEXT("%s date/time prints"), (m_bUseDate) ? (LOG_TEXT("Enabling")) : (LOG_TEXT("Disabling")));
			}

		/******************************************************************************
		Function :                      setUseDbgOut

		Description   : Enables/disables calls to OutputDebugString
		Parameters    : bUseDbg       - true enables, false disables
		Returns       : none
		******************************************************************************/
		void Logger::setUseDbgOut(bool bUseDbg)
			{
			/////////////////////////////////////////////////////
			ENTER_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			m_bEnableDbgOut = bUseDbg;
			/////////////////////////////////////////////////////
			LEAVE_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			if (m_bInitialized)
				Log(m_Level, LOG_TEXT("%s debugger prints"), (m_bEnableDbgOut) ? (LOG_TEXT("Enabling")) : (LOG_TEXT("Disabling")));
			}


		/******************************************************************************
		Function :			bUsethreadID

		Description   : Enables/disables thread identifier prints
		Parameters    : bUsethreadID    - true enables, false disables
		Returns       : none
		******************************************************************************/
		void Logger::setUseThreadID(bool bUsethreadID)
			{
			/////////////////////////////////////////////////////
			ENTER_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			m_bUseThreadID = bUsethreadID;
			/////////////////////////////////////////////////////
			LEAVE_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			if (m_bInitialized)
				Log(m_Level, LOG_TEXT("%s thread ID prints"), (m_bUseThreadID) ? (LOG_TEXT("Enabling")) : (LOG_TEXT("Disabling")));
			}


		/******************************************************************************
		Function :			bUsethreadID

		Description   : Enables/disables cutom callbacks to redirect output. IT OVERRIDES ALL OUTPUTS
		Returns       : none
		******************************************************************************/
		void Logger::setUseCustomOutput(bool bCustomOutput /*= false*/, CUSTOM_LOG_CALLBACK pCustomCallback)
			{
			/////////////////////////////////////////////////////
			ENTER_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			m_bCustomCB = bCustomOutput;
			m_pCustomCallback = pCustomCallback;
			/////////////////////////////////////////////////////
			LEAVE_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			if (m_bInitialized)
				Log(m_Level, LOG_TEXT("%s custom callback"), (m_bCustomCB) ? (LOG_TEXT("Enabling")) : (LOG_TEXT("Disabling")));
			}


		/******************************************************************************
		Function :                      close

		Description   : Closes the log file
		Parameters    : none
		Returns               : none
		******************************************************************************/
		void Logger::close()
			{
			/////////////////////////////////////////////////////
			ENTER_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			if (m_pFile)
				{
				fclose(m_pFile);
				m_pFile = NULL;
				m_bInitialized = false;
				}
			/////////////////////////////////////////////////////
			LEAVE_CRITICAL_SECTION(&g_LogCS);
			/////////////////////////////////////////////////////
			}

		/******************************************************************************
		Function :			getLevelString

		Description	: Translates a Level to the corresponding log_string
		Parameters	: level			- level to convert to log_string
		szOutString   - pointer to the log_string container
		Returns		:
		******************************************************************************/
		void Logger::getLevelString(Level level, log_string * szOutString)
			{
			switch (level)
				{
				case Level::FatalError:
					*szOutString = LOG_TEXT("::FER:: ");
					break;
				case Level::Error:
					*szOutString = LOG_TEXT("::ERR:: ");
					break;
				case Level::Warning:
					*szOutString = LOG_TEXT("::WRN:: ");
					break;
				case Level::Info:
					*szOutString = LOG_TEXT("::INF:: ");
					break;
				case Level::Call:
					*szOutString = LOG_TEXT("::FCL:: ");
					break;
				case Level::All:
					*szOutString = LOG_TEXT("::ALL:: ");
					break;
				default:
					*szOutString = LOG_TEXT("::UNK:: ");
				}
			}


		//+----------------------------------------------------------------------+
		void Logger::PrintOut(char * pData, int len)
			{
			if (m_bEnableDbgOut)
				OUTPUT_DEBUG_STRING(pData);

			AppendToFile(pData, len);

			if (m_bCustomCB && m_pCustomCallback)
				m_pCustomCallback(pData, len);

			if (m_bEnableCout)
				std::cout << pData;
			}


		//+----------------------------------------------------------------------+
		void Logger::AppendToFile(void * buffer, int buf_len)
			{
			if (m_bUseFileOut)
				{
				m_pFile = FOPEN(m_szFilename.c_str(), LOG_TEXT("a"));
				if (m_pFile)
					{
					fwrite(buffer, buf_len, 1, m_pFile);
					fflush(m_pFile);
					fclose(m_pFile);
					}
				}
			}






		}
	}


