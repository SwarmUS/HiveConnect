#include "BSP.h"
#include <FreeRTOS.h>
#include <task.h>
#include <thread>

BSP::~BSP() = default;

void rosSpinTask(void* param) {
    configASSERT(param != nullptr);

    unsigned int loopRate = *(unsigned int*)param;
    while (ros::ok()) {
        ros::spinOnce();
        vTaskDelay(loopRate);
    }
    vTaskEndScheduler();
}

BSP::BSP(const ros::NodeHandle& nodeHandle, const int loopRate, const ILogger& logger) :
    m_loopRate(loopRate) {
    m_rosNodeHandle = std::make_shared<ros::NodeHandle>(nodeHandle);
    m_logger = &logger;
}

void BSP::initChip() {
    xTaskCreate(rosSpinTask, "ros_spinner", configMINIMAL_STACK_SIZE, (void*)&this->m_loopRate,
                tskIDLE_PRIORITY + 1, NULL);
}

ChipInfo BSP::getChipInfo() {
    return ChipInfo{.m_cores = (uint8_t)std::thread::hardware_concurrency(),
                    .m_osType = ChipInfo::SIMULATION};
}

void BSP::log(LogLevel level, const char* format, ...) {
    va_list args;
    va_start(args, format);
    m_logger->log(level, format, args);
    va_end(args);
}

std::shared_ptr<ros::NodeHandle> BSP::getNodeHandle() { return m_rosNodeHandle; }
