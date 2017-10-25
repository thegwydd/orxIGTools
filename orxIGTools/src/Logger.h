#if !defined _LOGGER_INCLUDE_
#define _LOGGER_INCLUDE_

#include <spdlog/spdlog.h>

#define LOG_TEXT(x)	x
#define LOGGER_DEFAULT_BUFFER	(1500)
#define	BYTE_PER_LINE	32

extern std::shared_ptr<spdlog::logger> _LOGGER_INSTANCE;

#define _TRACE 		_LOGGER_INSTANCE->info("console");

#ifdef _MSC_VER
#define TRACE_LOG(...) _LOGGER_INSTANCE->trace(__VA_ARGS__)
#define TRACE_TRC(...) _LOGGER_INSTANCE->trace(__VA_ARGS__)
#define TRACE_DBG(...) _LOGGER_INSTANCE->debug(__VA_ARGS__)
#define TRACE_NFO(...) _LOGGER_INSTANCE->info(__VA_ARGS__)
#define TRACE_WRN(...) _LOGGER_INSTANCE->warn(__VA_ARGS__)
#define TRACE_ERR(...) _LOGGER_INSTANCE->error(__VA_ARGS__)
#define TRACE_CRT(...) _LOGGER_INSTANCE->critical(__VA_ARGS__)
#else
#define TRACE_LOG(...) _LOGGER_INSTANCE->trace( ## args)
#define TRACE_TRC(...) _LOGGER_INSTANCE->trace( ## args)
#define TRACE_DBG(...) _LOGGER_INSTANCE->debug( ## args)
#define TRACE_NFO(...) _LOGGER_INSTANCE->info( ## args)
#define TRACE_WRN(...) _LOGGER_INSTANCE->warn( ## args)
#define TRACE_ERR(...) _LOGGER_INSTANCE->error( ## args)
#define TRACE_CRT(...) _LOGGER_INSTANCE->critical( ## args)
#endif



#endif //_LOGGER_INCLUDE_
