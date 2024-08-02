#include "gpioMonitor.hpp"

#include "constants.hpp"
#include "types.hpp"
#include "utility/dbus_utility.hpp"
#include "utility/generic_utility.hpp"
#include "utility/json_utility.hpp"

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <gpiod.hpp>

namespace vpd
{
bool GpioEventHandler::getPresencePinValue()
{
    gpiod::line l_presenceLine = gpiod::find_line(m_presencePin);

    if (!l_presenceLine)
    {
        logging::logMessage("Couldn't find presence line: " + m_presencePin);
        return m_prevPresencePinValue;
    }

    l_presenceLine.request(
        {"Presnce line", gpiod::line_request::DIRECTION_INPUT, 0});
    uint8_t l_presencePinValue = l_presenceLine.get_value();

    return l_presencePinValue;
}

void GpioEventHandler::toggleGpio()
{
    bool l_isPresent = false;

    if (getPresencePinValue() == m_presenceValue)
    {
        l_isPresent = true;
    }

    // if FRU went away set the present property to false
    if (!l_isPresent)
    {
        types::ObjectMap l_objectMap;
        types::InterfaceMap l_interfaces;
        types::PropertyMap l_presentProp;

        l_presentProp.emplace("Present", false);
        l_interfaces.emplace("xyz.openbmc_project.Inventory.Item",
                             l_presentProp);
        l_objectMap.emplace(m_inventoryPath, move(l_interfaces));

        if (!dbusUtility::callPIM(move(l_objectMap)))
        {
            logging::logMessage("Call to PIM failed for path: " +
                                m_inventoryPath);
            return;
        }
    }

    gpiod::line l_outputLine = gpiod::find_line(m_outputPin);
    if (!l_outputLine)
    {
        logging::logMessage("couldn't find output line:" + m_outputPin +
                            ". Skipping update");
        return;
    }

    l_outputLine.request({"FRU presence: update the output GPIO pin",
                          gpiod::line_request::DIRECTION_OUTPUT, 0},
                         l_isPresent ? m_outputValue : (!m_outputValue));

    if (!l_isPresent)
    {
        m_bindorUnbindCmd = m_bindorUnbindCmd.replace(
            m_bindorUnbindCmd.find("bind"), 4, "unbind");
    }

    genericUtility::executeCmd(m_bindorUnbindCmd);
}

void GpioEventHandler::handleTimerExpiry(
    const boost::system::error_code& i_errorCode,
    const std::shared_ptr<boost::asio::steady_timer>& i_timerObj)
{
    if (i_errorCode == boost::asio::error::operation_aborted)
    {
        return;
    }

    if (i_errorCode)
    {
        logging::logMessage("Timer wait failed for gpio pin" +
                            i_errorCode.message());
        return;
    }

    if (hasEventOccurred())
    {
        toggleGpio();
    }

    i_timerObj->expires_at(std::chrono::steady_clock::now() +
                           std::chrono::seconds(5));
    i_timerObj->async_wait(boost::bind(&GpioEventHandler::handleTimerExpiry,
                                       this, boost::asio::placeholders::error,
                                       i_timerObj));
}

void GpioEventHandler::setEventHandlerForGpioPresence(
    const std::shared_ptr<boost::asio::io_context>& i_ioContext)
{
    m_prevPresencePinValue = getPresencePinValue();

    static std::vector<std::shared_ptr<boost::asio::steady_timer>> l_timers;

    auto l_timerObj = make_shared<boost::asio::steady_timer>(
        *i_ioContext, std::chrono::seconds(5));

    l_timerObj->async_wait(boost::bind(&GpioEventHandler::handleTimerExpiry,
                                       this, boost::asio::placeholders::error,
                                       l_timerObj));

    l_timers.push_back(l_timerObj);
}
} // namespace vpd
