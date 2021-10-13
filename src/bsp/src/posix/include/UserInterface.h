#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "bsp/IBSP.h"
#include "bsp/IUserInterface.h"
#include <string>
#include <utility>

class UserInterface : public IUserInterface {
  public:
    UserInterface(const IBSP& bsp);

    ~UserInterface() override = default;

    int print(const char* format, ...) const override;

    int print(const char* format, va_list args) const override;

    int printInfo(const char* format, va_list args) const override;

    int printDebug(const char* format, va_list args) const override;

    int printWarning(const char* format, va_list args) const override;

    int printError(const char* format, va_list args) const override;

    void setNetworkRGB(RGBColor color) override;

  private:
    static std::pair<std::string, int> generateBuffer(const char* format, va_list args);
    const IBSP& m_bsp;
};

#endif // HIVE_CONNECT_USERINTERFACE_H
