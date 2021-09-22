#ifndef HIVE_CONNECT_IRGB_H
#define HIVE_CONNECT_IRGB_H

enum class RGBColor { RED = 0, GREEN, BLUE, VIOLET, TEAL, YELLOW, WHITE, OFF };

class IRGB {
  public:
    virtual ~IRGB() = default;
    virtual void setColor(RGBColor color) = 0;
};

#endif // HIVE_CONNECT_IRGB_H
