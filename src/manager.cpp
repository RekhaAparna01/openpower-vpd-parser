#include "manager.hpp"

namespace vpd
{
Manager::Manager(const std::shared_ptr<boost::asio::io_context>& ioCon,
                 const std::shared_ptr<sdbusplus::asio::dbus_interface>& iFace,
                 const std::shared_ptr<sdbusplus::asio::connection>& conn) :
    m_ioContext(ioCon),
    m_interface(iFace), m_conn(conn)
{
}
} // namespace vpd