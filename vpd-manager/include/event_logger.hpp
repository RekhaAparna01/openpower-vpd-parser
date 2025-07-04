#pragma once

#include "constants.hpp"
#include "types.hpp"

#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

namespace vpd
{
/**
 * @brief Class for logging events.
 *
 * Class handles logging PEL under 'logging' service.
 * Provide separate async API's for calling out inventory_path, device_path and
 * i2c bus.
 */
class EventLogger
{
  public:
    /**
     * @brief API to get Error type.
     *
     * @param[in] i_exception - Exception object.
     *
     * @return Error type set for the exception.
     * types::ErrorType::InternalFailure otherwise.
     */
    static types::ErrorType getErrorType(const std::exception& i_exception);

    /**
     * @brief API to get Error msg.
     *
     * @param[in] i_exception - Exception object.
     *
     * @return Error msg set for the specific exception. Default error msg
     * otherwise.
     */
    static std::string getErrorMsg(const std::exception& i_exception);

    /**
     * @brief API to get string representation of a Error type enum.
     *
     * @param[in] i_exception - Exception object.
     *
     * @return Error msg set for the specific error type. Default error msg
     * otherwise.
     */
    static std::string getErrorTypeString(
        const types::ErrorType& i_errorType) noexcept;

    /**
     * @brief An API to create a PEL with inventory path callout.
     *
     * This API calls an async method to create PEL, and also handles inventory
     * path callout.
     *
     * Note: If inventory path callout info is not provided, it will create a
     * PEL without any callout. Currently only one callout is handled in this
     * API.
     *
     * @todo: Symbolic FRU and procedure callout needs to be handled in this
     * API.
     *
     * @param[in] i_errorType - Enum to map with event message name.
     * @param[in] i_severity - Severity of the event.
     * @param[in] i_callouts - Callout information, list of tuple having
     * inventory path and priority as input [optional].
     * @param[in] i_fileName - File name.
     * @param[in] i_funcName - Function name.
     * @param[in] i_internalRc - Internal return code.
     * @param[in] i_description - Error description.
     * @param[in] i_userData1 - Additional user data [optional].
     * @param[in] i_userData2 - Additional user data [optional].
     * @param[in] i_symFru - Symblolic FRU callout data [optional].
     * @param[in] i_procedure - Procedure callout data [optional].
     *
     * @throw exception in case of error.
     */
    static void createAsyncPelWithInventoryCallout(
        const types::ErrorType& i_errorType,
        const types::SeverityType& i_severity,
        const std::vector<types::InventoryCalloutData>& i_callouts,
        const std::string& i_fileName, const std::string& i_funcName,
        const uint8_t i_internalRc, const std::string& i_description,
        const std::optional<std::string> i_userData1,
        const std::optional<std::string> i_userData2,
        const std::optional<std::string> i_symFru,
        const std::optional<std::string> i_procedure);

    /**
     * @brief An API to create a PEL with device path callout.
     *
     * @param[in] i_errorType - Enum to map with event message name.
     * @param[in] i_severity - Severity of the event.
     * @param[in] i_callouts - Callout information, list of tuple having device
     * path and error number as input.
     * @param[in] i_fileName - File name.
     * @param[in] i_funcName - Function name.
     * @param[in] i_internalRc - Internal return code.
     * @param[in] i_userData1 - Additional user data [optional].
     * @param[in] i_userData2 - Additional user data [optional].
     */
    static void createAsyncPelWithI2cDeviceCallout(
        const types::ErrorType i_errorType,
        const types::SeverityType i_severity,
        const std::vector<types::DeviceCalloutData>& i_callouts,
        const std::string& i_fileName, const std::string& i_funcName,
        const uint8_t i_internalRc,
        const std::optional<std::pair<std::string, std::string>> i_userData1,
        const std::optional<std::pair<std::string, std::string>> i_userData2);

    /**
     * @brief An API to create a PEL with I2c bus callout.
     *
     * @param[in] i_errorType - Enum to map with event message name.
     * @param[in] i_severity - Severity of the event.
     * @param[in] i_callouts - Callout information, list of tuple having i2c
     * bus, i2c address and error number as input.
     * @param[in] i_fileName - File name.
     * @param[in] i_funcName - Function name.
     * @param[in] i_internalRc - Internal return code.
     * @param[in] i_userData1 - Additional user data [optional].
     * @param[in] i_userData2 - Additional user data [optional].
     */
    static void createAsyncPelWithI2cBusCallout(
        const types::ErrorType i_errorType,
        const types::SeverityType i_severity,
        const std::vector<types::I2cBusCalloutData>& i_callouts,
        const std::string& i_fileName, const std::string& i_funcName,
        const uint8_t i_internalRc,
        const std::optional<std::pair<std::string, std::string>> i_userData1,
        const std::optional<std::pair<std::string, std::string>> i_userData2);

    /**
     * @brief An API to create a PEL.
     *
     * @param[in] i_errorType - Enum to map with event message name.
     * @param[in] i_severity - Severity of the event.
     * @param[in] i_fileName - File name.
     * @param[in] i_funcName - Function name.
     * @param[in] i_internalRc - Internal return code.
     * @param[in] i_description - Error description.
     * @param[in] i_userData1 - Additional user data [optional].
     * @param[in] i_userData2 - Additional user data [optional].
     * @param[in] i_symFru - Symblolic FRU callout data [optional].
     * @param[in] i_procedure - Procedure callout data [optional].
     *
     * @todo: Symbolic FRU and procedure callout needs to be handled in this
     * API.
     */
    static void createAsyncPel(
        const types::ErrorType& i_errorType,
        const types::SeverityType& i_severity, const std::string& i_fileName,
        const std::string& i_funcName, const uint8_t i_internalRc,
        const std::string& i_description,
        const std::optional<std::string> i_userData1,
        const std::optional<std::string> i_userData2,
        const std::optional<std::string> i_symFru,
        const std::optional<std::string> i_procedure);

    /**
     * @brief An API to create PEL.
     *
     * This API makes synchronous call to phosphor-logging Create method.
     *
     * @param[in] i_errorType - Enum to map with event message name.
     * @param[in] i_severity - Severity of the event.
     * @param[in] i_fileName - File name.
     * @param[in] i_funcName - Function name.
     * @param[in] i_internalRc - Internal return code.
     * @param[in] i_description - Error description.
     * @param[in] i_userData1 - Additional user data [optional].
     * @param[in] i_userData2 - Additional user data [optional].
     * @param[in] i_symFru - Symblolic FRU callout data [optional].s
     * @param[in] i_procedure - Procedure callout data [optional].
     *
     * @todo: Symbolic FRU and procedure callout needs to be handled in this
     * API.
     */
    static void createSyncPel(
        const types::ErrorType& i_errorType,
        const types::SeverityType& i_severity, const std::string& i_fileName,
        const std::string& i_funcName, const uint8_t i_internalRc,
        const std::string& i_description,
        const std::optional<std::string> i_userData1,
        const std::optional<std::string> i_userData2,
        const std::optional<std::string> i_symFru,
        const std::optional<std::string> i_procedure);

    /**
     * @brief An API to create a synchronus PEL with inventory path callout.
     *
     * This API calls phosphor-logging method to create PEL, and also handles
     * inventory path callout. In case called with EEPROM path, will look for
     * JSON at symbolic link and if present will fetch inventory path for that
     * EEPROM.
     *
     * Note: In case of any error in fetching JSON or converting the EEPROM
     * path, the API will log PEL with path passed in callout parameter.
     * If inventory/EEPROM path is not provided in the callout, it will create
     * PEL without call out. Currently only one callout is handled in this API.
     *
     * @todo: Symbolic FRU and procedure callout needs to be handled in this
     * API.
     *
     * @param[in] i_errorType - Enum to map with event message name.
     * @param[in] i_severity - Severity of the event.
     * @param[in] i_fileName - File name.
     * @param[in] i_funcName - Function name.
     * @param[in] i_internalRc - Internal return code.
     * @param[in] i_description - Error description.
     * @param[in] i_callouts - Callout information.
     * @param[in] i_userData1 - Additional user data [optional].
     * @param[in] i_userData2 - Additional user data [optional].
     * @param[in] i_symFru - Symblolic FRU callout data [optional].
     * @param[in] i_procedure - Procedure callout data [optional].
     *
     */
    static void createSyncPelWithInvCallOut(
        const types::ErrorType& i_errorType,
        const types::SeverityType& i_severity, const std::string& i_fileName,
        const std::string& i_funcName, const uint8_t i_internalRc,
        const std::string& i_description,
        const std::vector<types::InventoryCalloutData>& i_callouts,
        const std::optional<std::string> i_userData1,
        const std::optional<std::string> i_userData2,
        [[maybe_unused]] const std::optional<std::string> i_symFru,
        [[maybe_unused]] const std::optional<std::string> i_procedure);

  private:
    /**
     * @brief API to get error info based on the exception.
     *
     * @param[in] i_exception - Exception object.
     *
     * @return - Valid ExceptionDataMap on success, otherwise map having
     * default value.
     */
    static types::ExceptionDataMap getExceptionData(
        const std::exception& i_exception);

    static const std::unordered_map<types::SeverityType, std::string>
        m_severityMap;
    static const std::unordered_map<types::ErrorType, std::string>
        m_errorMsgMap;
    static const std::unordered_map<types::CalloutPriority, std::string>
        m_priorityMap;
};
} // namespace vpd
