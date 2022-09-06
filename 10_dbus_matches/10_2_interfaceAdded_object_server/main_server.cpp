#include <boost/asio.hpp>
#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/asio/connection.hpp>
#include <sdbusplus/asio/object_server.hpp>
#include <sdbusplus/asio/property.hpp>
#include <sdbusplus/bus/match.hpp>

PHOSPHOR_LOG2_USING;

int main(int, char const**)
{
    boost::asio::io_context io;
    auto bus = std::make_shared<sdbusplus::asio::connection>(io);
    sdbusplus::asio::object_server objectServer(bus);

    auto myInterface =
        objectServer.add_interface("/com", "com.bytedance.my_interface");
    myInterface->initialize();

    auto myInterface1 = objectServer.add_interface(
        "/com/bytedance", "com.bytedance.my_interface");
    myInterface1->initialize();

    auto myInterface2 = objectServer.add_interface(
        "/com/bytedance/my_interface_path", "com.bytedance.my_interface");
    myInterface2->initialize();

    auto myInterface3 = objectServer.add_interface(
        "/com/bytedance/my_interface_path/sub1", "com.bytedance.my_interface");
    myInterface3->initialize();

    bus->request_name("com.bytedance.my_test_aiso_service");

    info("Bus initialized");
    io.run();

    return 0;
}
