#if !defined _LOGGER_INCLUDE_
#define _LOGGER_INCLUDE_

#include "Types.h"
#include "Singleton.h"
#include <string>
#include <vector>

#define LOG_TEXT(x)	x
#define LOGGER_DEFAULT_BUFFER	(1500)
#define	BYTE_PER_LINE	32

#define LOGGER_NAMESPACE orxSmith::Core

#define _TRACE 		LOGGER_NAMESPACE::Logger::Instance().Log
#define _TRACE2		LOGGER_NAMESPACE::Logger::Instance().Log2
#define _TRACE_BUF	LOGGER_NAMESPACE::Logger::Instance().LogBuffer


#ifdef _MSC_VER
#define TRACE_CALL(...) _TRACE(LOGGER_NAMESPACE::Logger::Level::Call, __VA_ARGS__)
#define TRACE_DBG(...) _TRACE(LOGGER_NAMESPACE::Logger::Level::Info, __VA_ARGS__)
#define TRACE_WRN(...) _TRACE(LOGGER_NAMESPACE::Logger::Level::Warning, __VA_ARGS__)
#define TRACE_NFO(...) _TRACE(LOGGER_NAMESPACE::Logger::Level::Info, __VA_ARGS__)
#define TRACE_ERR(...) _TRACE(LOGGER_NAMESPACE::Logger::Level::Error, __VA_ARGS__)
#define TRACE_BUF(...) _TRACE_BUF(LOGGER_NAMESPACE::Logger::Level::Info, __VA_ARGS__)
#else
#define TRACE_CALL(args...) _TRACE(LOGGER_NAMESPACE::Logger::Level::Call, ## args)
#define TRACE_DBG(args...) _TRACE(LOGGER_NAMESPACE::Logger::Level::Info, ## args)
#define TRACE_WRN(args...) _TRACE(LOGGER_NAMESPACE::Logger::Level::Warning, ## args)
#define TRACE_NFO(args...) _TRACE(LOGGER_NAMESPACE::Logger::Level::Info, ## args)
#define TRACE_ERR(args...) _TRACE(LOGGER_NAMESPACE::Logger::Level::Error, ## args)
#define TRACE_BUF(args...) _TRACE_BUF(LOGGER_NAMESPACE::Logger::Level::Info, ## args)

#endif
#define _TRACE_FUNC(name) _TRACE(LOGGER_NAMESPACE::Logger::Level::Call, LOG_TEXT("Calling : %s"), name)

#ifdef EXTRA_LOG
#define X_TRACE 		Logger::Instance().Log
#else
#define X_TRACE		__noop
#endif

#if defined(linux)
#define OutputDebugString(x) printf(LOG_TEXT("%s"), x)
#endif


using namespace std;

#ifdef WIN32
#define TICK_COUNT ::GetTickCount()
#define USEC_COUNT ::GetTickCount()
#else
#include <sys/time.h>
static inline unsigned long _get_tick_count() {
	timeval ts; gettimeofday(&ts,0); return (long)(ts.tv_sec * 1000 + (ts.tv_usec / 1000));
	}
static inline uint64 _get_usec_count() {
	timeval ts; gettimeofday(&ts,0); return ((uint64)ts.tv_sec * 1000000) + (uint64)ts.tv_usec;
	}
#define GetTickCount _get_tick_count

#define TICK_COUNT _get_tick_count()
#define USEC_COUNT _get_usec_count()
#endif 


#ifdef LOG_USE_UNICODE
#define log_string		std::wstring
#define log_char		std::wstring::value_type
#else
#define log_string		std::string
#define log_char		std::string::value_type
#endif

namespace orxSmith
	{
	namespace Core
		{

		typedef int(*CUSTOM_LOG_CALLBACK)(char * p_buffer, int buffer_len);




		//======================================================================================
		//                               Logger
		//======================================================================================
		// Description : Logger utility class
		//======================================================================================
		class Logger : public Singleton<Logger>
			{
			public:
				Logger(log_string filename = LOG_TEXT(""), bool use_debug = true, bool use_date = true, bool use_file = false, bool use_cout = true);
				~Logger(void);

				enum class Level
					{
					FatalError = 0,
					Error,
					Warning,
					Info,
					Call,
					All
					};

			private:
				log_string			m_szFilename;
				FILE *				m_pFile;
				bool				m_bInitialized;
				bool        		m_bUseDate;
				bool				m_bEnableDbgOut;
				bool				m_bSocketDbgOut;
				bool				m_bUseFileOut;
				bool        		m_bUseThreadID;
				bool        		m_bCustomCB;
				Level    			m_Level;
				bool				m_bEnableCout;
				CUSTOM_LOG_CALLBACK m_pCustomCallback;

				bool				initLogger(log_string szFilename = LOG_TEXT(""), Level level = Level::All);

				bool				InitializeFile(log_string filename);
				void				AppendToFile(void * buffer, int buf_len);
				void				GetTime(log_char * pBuffer, int iBufLen);

				vector<log_string>	get_buffer_lines(const void * buffer, int buf_len);
				vector<log_string>	get_buffer_lines(log_string buffer);

			public:
				void				setLevel(int level);
				void				setUseFile(bool bUseFile, log_string filename);
				void				setUseDate(bool bUseDate = true);
				void				setUseCustomOutput(bool bCustomOutput = false, CUSTOM_LOG_CALLBACK pCustomCallback = NULL);
				void				setUseDbgOut(bool bUseDbg = true);
				void				setUseThreadID(bool bUsethreadID = true);
				void				close();
				void				getLevelString(Level level, log_string * szOutString);
				log_string			getDefaultFilename();
				void				Log(Level level, const log_char * pszFormat, ...);
				void				Log2(const log_char * szFormat, ...);
				void				LogDbg(log_string file, int line, const log_char * pszFormat, ...);
				void				LogBuffer(Level level, const log_char *Text, const void * buffer, int buf_len);
				void				LogBuffer(Level level, const log_char *Text, log_string buffer);
				void 				PrintOut(log_char * pData, int len);

			};

		}
	}


#endif //_LOGGER_INCLUDE_
