#include <boost/asio.hpp>
#include <com/bytedance/test/server.hpp>
#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/asio/connection.hpp>
#include <sdbusplus/asio/object_server.hpp>
#include <sdbusplus/asio/property.hpp>
#include <sdbusplus/bus/match.hpp>

#include <iostream>

namespace com::bytedance::test
{
using TestInherit =
    sdbusplus::server::object::object<sdbusplus::com::bytedance::server::test>;

class Test : public TestInherit
{
  public:
    Test(sdbusplus::bus_t& bus, const char* path) : TestInherit(bus, path)
    {
        std::cout << "Test::Test()" << std::endl;
        myProperty("myProperty");
    }
    ~Test()
    {
        std::cout << "Test::~Test()" << std::endl;
    }
};
} // namespace com::bytedance::test

int main(int, char const**)
{
    boost::asio::io_context io;
    auto bus = std::make_shared<sdbusplus::asio::connection>(io);
    // If you want emit a interfaces added/removed signal, add this before
    // create object
    sdbusplus::server::manager_t objManager(*bus,
                                            "/com/bytedance/my_interface_path");

    com::bytedance::test::Test test(*bus, "/com/bytedance/my_interface_path");
    com::bytedance::test::Test test1(*bus,
                                     "/com/bytedance/my_interface_path/sub1");
    com::bytedance::test::Test test2(
        *bus, "/com/bytedance/my_interface_path/sub1/subsub1");

    bus->request_name("com.bytedance.my_test_aiso_service");
    io.run();

    return 0;
}
