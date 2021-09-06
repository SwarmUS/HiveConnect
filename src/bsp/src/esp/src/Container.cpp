#include "bsp/Container.h"
#include "BSP.h"
#include "SpiStm.h"
#include "Storage.h"
#include "UserInterface.h"
#include "logger/LoggerContainer.h"

namespace BspContainer {

IBSP& getBSP() {
    static BSP s_bsp;

    return s_bsp;
}

IUserInterface& getUserInterface() {
    static UserInterface s_ui;

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
} // namespace BspContainer