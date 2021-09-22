#ifndef HIVE_CONNECT_RGB_H
#define HIVE_CONNECT_RGB_H

#include "bsp/IRGB.h"
#include <cstdint>

class RGB : public IRGB {
  public:
    RGB(uint8_t red, uint8_t green, uint8_t blue);
    ~RGB() override = default;

    void setColor(RGBColor color) override;

  private:
    uint8_t m_redGPIO, m_greenGPIO, m_blueGPIO;
};

#endif // HIVE_CONNECT_RGB_H
