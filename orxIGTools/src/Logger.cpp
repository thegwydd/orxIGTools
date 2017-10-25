#include "Logger.h"

#include <spdlog/spdlog.h>

std::shared_ptr<spdlog::logger> _LOGGER_INSTANCE = spdlog::stdout_color_mt("console");