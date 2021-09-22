#include "UserInterface.h"
#include "ros/ros.h"
#include <cstdarg>

UserInterface::UserInterface(const IBSP& bsp) : m_bsp(bsp) {}

int UserInterface::print(const char* format, va_list args) const {
    int ret = vprintf(format, args);
    printf("\n\r");
    return ret;
}

int UserInterface::print(const char* format, ...) const {
    va_list args;
    va_start(args, format);
    int retValue = print(format, args);
    va_end(args);

    return retValue;
}

int UserInterface::printInfo(const char* format, va_list args) const {
    std::pair<std::string, int> statement = UserInterface::generateBuffer(format, args);
    ROS_INFO("[HC: %d] %s", m_bsp.getHiveMindUUID(), statement.first.data());
    return statement.second;
}

int UserInterface::printDebug(const char* format, va_list args) const {
    std::pair<std::string, int> statement = UserInterface::generateBuffer(format, args);
    ROS_DEBUG("[HC: %d] %s", m_bsp.getHiveMindUUID(), statement.first.data());
    return statement.second;
};

int UserInterface::printWarning(const char* format, va_list args) const {
    std::pair<std::string, int> statement = UserInterface::generateBuffer(format, args);
    ROS_WARN("[HC: %d] %s", m_bsp.getHiveMindUUID(), statement.first.data());
    return statement.second;
};

int UserInterface::printError(const char* format, va_list args) const {
    std::pair<std::string, int> statement = UserInterface::generateBuffer(format, args);
    ROS_ERROR("[HC: %d] %s", m_bsp.getHiveMindUUID(), statement.first.data());
    return statement.second;
}

std::pair<std::string, int> UserInterface::generateBuffer(const char* format, va_list args) {
    std::pair<std::string, int> retVal;

    va_list vaCopy;
    va_copy(vaCopy, args);
    const int requiredLength = std::vsnprintf(NULL, 0, format, vaCopy);
    va_end(vaCopy);

    // Create a string with adequate length
    retVal.first.resize((size_t)requiredLength);

    // Build a new string
    retVal.second = vsnprintf(retVal.first.data(), retVal.first.size() + 1, format, args);
    return retVal;
}

void UserInterface::setNetworkRGB(RGBColor color) {
    ros::NodeHandle node("~");
    switch (color) {
    case RGBColor::RED:
        node.setParam("NetworkStatus", "Error");
        break;
    case RGBColor::GREEN:
        node.setParam("NetworkStatus", "Connected");
        break;
    case RGBColor::TEAL:
        node.setParam("NetworkStatus", "Access point");
        break;
    case RGBColor::YELLOW:
        node.setParam("NetworkStatus", "Connecting");
        break;
    case RGBColor::VIOLET:
    case RGBColor::BLUE:
    case RGBColor::WHITE:
    case RGBColor::OFF:
        node.setParam("NetworkStatus", "Unknown");
        break;
    }
}