#include "bsp/Container.h"
#include "BSP.h"
#include "RGB.h"
#include "SpiStm.h"
#include "Storage.h"
#include "UserInterface.h"
#include "hal/pin_map.h"
#include "logger/LoggerContainer.h"

namespace BspContainer {

IBSP& getBSP() {
    static BSP s_bsp;

    return s_bsp;
}

IUserInterface& getUserInterface() {
    static UserInterface s_ui(getNetworkRGB());

    return s_ui;
}

ISpiStm& getSpiStm() {
    static SpiStm s_spiStm(LoggerContainer::getLogger());

    return s_spiStm;
}

IStorage& getStorage() {
    static Storage s_storage(LoggerContainer::getLogger());

    return s_storage;
}

IRGB& getNetworkRGB() {
    static RGB s_rgb(RGB_R, RGB_G, RGB_B);

    return s_rgb;
}
} // namespace BspContainer