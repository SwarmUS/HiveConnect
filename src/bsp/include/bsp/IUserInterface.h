#ifndef __IUSERINTERFACE_H_
#define __IUSERINTERFACE_H_

#include "IRGB.h"
#include <cstdarg>

/**
 * @brief Manages the user interface
 * The user interface can consist of buttons, LED, serial print ports, etc.
 * It is not a graphical interface.
 * The board will communicate its state via the UI and the user can send commands via the UI.
 */

class IUserInterface {
  public:
    virtual ~IUserInterface() = default;

    /**
     * @brief Provides an interface to print to the console or serial port.
     * The arguments and return values match the standard printf library
     *
     * @param [in] format Text to be written, can contain format specifiers that will be replaced by
     *values specified in the additional arguments, matches the standard printf function
     *
     * @param [in] ... Additionnal arguments for the format parameter
     *
     * @return Matches the standard printf return. The total number of characters is returned or a
     *negative number on error
     */
    virtual int print(const char* format, ...) const = 0;

    /**
     * @brief Provides an interface to print to the console or serial port using an initialized
     *va_list. The return value matches the standard printf library.
     *
     * @param [in] format Text to be written, can contain format specifiers that will be replaced by
     *values specified in the additional arguments, matches the standard printf function
     *
     * @param [in] args Previously initialized va_list
     *
     * @return Matches the standard printf return. The total number of characters is returned or a
     *negative number on error
     */
    virtual int print(const char* format, va_list args) const = 0;

    /**
     * @brief Prints with the INFO log level.
     *
     * @param [in] format Text to be written, can contain format specifiers that will be replaced by
     *values specified in the additional arguments, matches the standard printf function
     * @param args [in] Previously initialized va_list
     * @return Matches the standard printf return. The total number of characters is returned or a
     *negative number on error
     */
    virtual int printInfo(const char* format, va_list args) const = 0;

    /**
     * @brief Prints with the DEBUG log level.
     *
     * @param [in] format Text to be written, can contain format specifiers that will be replaced by
     *values specified in the additional arguments, matches the standard printf function
     * @param [in] args Previously initialized va_list
     * @return Matches the standard printf return. The total number of characters is returned or a
     *negative number on error
     */
    virtual int printDebug(const char* format, va_list args) const = 0;

    /**
     * @brief Prints with the WARNING log level.
     *
     * @param [in] format Text to be written, can contain format specifiers that will be replaced by
     *values specified in the additional arguments, matches the standard printf function
     * @param [in] args  Previously initialized va_list
     * @return Matches the standard printf return. The total number of characters is returned or a
     *negative number on error
     */
    virtual int printWarning(const char* format, va_list args) const = 0;

    /**
     * @brief Prints with the ERROR log level.
     *
     * @param [in] format Text to be written, can contain format specifiers that will be replaced by
     *values specified in the additional arguments, matches the standard printf function
     * @param [in] args Previously initialized va_list
     * @return Matches the standard printf return. The total number of characters is returned or a
     *negative number on error
     */
    virtual int printError(const char* format, va_list args) const = 0;

    /**
     * @brief Set the UI to display the networkState
     */
    virtual void setNetworkRGB(RGBColor color) = 0;
};

#endif // __IUSERINTERFACE_H_
