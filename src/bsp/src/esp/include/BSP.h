#ifndef BSP_H
#define BSP_H

#include "bsp/IBSP.h"
#include "bsp/IRGB.h"
#include <array>

class BSP : public IBSP {
  public:
    BSP();
    ~BSP() override = default;

    void initChip() override;
    ChipInfo getChipInfo() const override;
    uint16_t getHiveMindUUID() const override;
    void setHiveMindUUID(uint16_t uuid) override;

  private:
    void initSPI();
    void initRGB();
    uint16_t m_UUID;
};

#endif // BSP_H
