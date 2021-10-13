#include "RGB.h"
#include "driver/gpio.h"

RGB::RGB(uint8_t red, uint8_t green, uint8_t blue) :
    m_redGPIO(red), m_greenGPIO(green), m_blueGPIO(blue) {
    setColor(RGBColor::RED);
}

void RGB::setColor(RGBColor color) {
    switch (color) {
    case RGBColor::RED:
        gpio_set_level(static_cast<gpio_num_t>(m_redGPIO), 0);
        gpio_set_level(static_cast<gpio_num_t>(m_greenGPIO), 1);
        gpio_set_level(static_cast<gpio_num_t>(m_blueGPIO), 1);
        break;
    case RGBColor::GREEN:
        gpio_set_level(static_cast<gpio_num_t>(m_redGPIO), 1);
        gpio_set_level(static_cast<gpio_num_t>(m_greenGPIO), 0);
        gpio_set_level(static_cast<gpio_num_t>(m_blueGPIO), 1);
        break;
    case RGBColor::BLUE:
        gpio_set_level(static_cast<gpio_num_t>(m_redGPIO), 1);
        gpio_set_level(static_cast<gpio_num_t>(m_greenGPIO), 1);
        gpio_set_level(static_cast<gpio_num_t>(m_blueGPIO), 0);
        break;
    case RGBColor::VIOLET:
        gpio_set_level(static_cast<gpio_num_t>(m_redGPIO), 0);
        gpio_set_level(static_cast<gpio_num_t>(m_greenGPIO), 1);
        gpio_set_level(static_cast<gpio_num_t>(m_blueGPIO), 0);
        break;
    case RGBColor::TEAL:
        gpio_set_level(static_cast<gpio_num_t>(m_redGPIO), 1);
        gpio_set_level(static_cast<gpio_num_t>(m_greenGPIO), 0);
        gpio_set_level(static_cast<gpio_num_t>(m_blueGPIO), 0);
        break;
    case RGBColor::YELLOW:
        gpio_set_level(static_cast<gpio_num_t>(m_redGPIO), 0);
        gpio_set_level(static_cast<gpio_num_t>(m_greenGPIO), 0);
        gpio_set_level(static_cast<gpio_num_t>(m_blueGPIO), 1);
        break;
    case RGBColor::WHITE:
        gpio_set_level(static_cast<gpio_num_t>(m_redGPIO), 0);
        gpio_set_level(static_cast<gpio_num_t>(m_greenGPIO), 0);
        gpio_set_level(static_cast<gpio_num_t>(m_blueGPIO), 0);
        break;
    case RGBColor::OFF:
        gpio_set_level(static_cast<gpio_num_t>(m_redGPIO), 1);
        gpio_set_level(static_cast<gpio_num_t>(m_greenGPIO), 1);
        gpio_set_level(static_cast<gpio_num_t>(m_blueGPIO), 1);
        break;
    }
}