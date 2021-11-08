#include "BSP.h"
#include <Task.h>
#include <thread>

constexpr uint8_t gc_spinnerCores = 1;

/**
 * @brief Task that kills OS when ROS node is stopped
 */
void rosWatcher(void* param) {
    (void)param;
    const int loopRate = 100;

    while (ros::ok()) {
        ros::spinOnce();
        Task::delay(loopRate);
    }
}

BSP::~BSP() = default;

BSP::BSP(const ros::NodeHandle& nodeHandle) :
    m_rosWatchTask("ros_watch", tskIDLE_PRIORITY + 1, rosWatcher, NULL),
    m_spinner(gc_spinnerCores) {
    m_rosNodeHandle = std::make_shared<ros::NodeHandle>(nodeHandle);
}

void BSP::initChip() { m_rosWatchTask.start(); }

ChipInfo BSP::getChipInfo() const {
    return ChipInfo{.m_cores = (uint8_t)std::thread::hardware_concurrency(),
                    .m_osType = ChipInfo::SIMULATION};
}

std::shared_ptr<ros::NodeHandle> BSP::getNodeHandle() { return m_rosNodeHandle; }

uint16_t BSP::getHiveMindUUID() const { return m_UUID; }

void BSP::setHiveMindUUID(uint16_t uuid) { m_UUID = uuid; }
