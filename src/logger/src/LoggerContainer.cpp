#include "logger/LoggerContainer.h"
#include "bsp/Container.h"

Logger& LoggerContainer::getLogger() {
    static Logger s_logger = Logger(LogLevel::Warn, BspContainer::getUserInterface());
    return s_logger;
}
