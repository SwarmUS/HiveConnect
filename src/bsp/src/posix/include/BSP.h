#ifndef BSP_H
#define BSP_H

#include "BaseTask.h"
#include "bsp/IBSP.h"
#include "ros/ros.h"

class BSP : public IBSP {
  public:
    BSP(const ros::NodeHandle& nodeHandle);
    ~BSP() override;

    void initChip() override;
    ChipInfo getChipInfo() const override;
    uint16_t getHiveMindUUID() const override;
    void setHiveMindUUID(uint16_t uuid) override;

    std::shared_ptr<ros::NodeHandle> getNodeHandle();

  private:
    BaseTask<2 * configMINIMAL_STACK_SIZE> m_rosWatchTask;
    ros::AsyncSpinner m_spinner;
    std::shared_ptr<ros::NodeHandle> m_rosNodeHandle;
    uint16_t m_UUID;
};

#endif //_BSP_H
