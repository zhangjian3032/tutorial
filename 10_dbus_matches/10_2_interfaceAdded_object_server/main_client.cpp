#include <boost/asio.hpp>
#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/asio/connection.hpp>
#include <sdbusplus/asio/object_server.hpp>
#include <sdbusplus/asio/property.hpp>
#include <sdbusplus/bus/match.hpp>

PHOSPHOR_LOG2_USING;

namespace rules = sdbusplus::bus::match::rules;

using propertyVariantType = std::variant<std::string, bool, int>;
using propertyChangesType = std::map<std::string, propertyVariantType>;

// The server is sdbusplus::asio::object_server objectServer(bus);
// don't specify the manager object path, use the default value "/"
// This is the same as the server in
// 10_1_interfaceAdded_yaml_object_interface_manager_object/main_server.cpp
// and equal to sdbusplus::server::manager_t objManager(*bus, "/");

int main(int /*argc*/, char** /*argv*/)
{
    boost::asio::io_context io;
    auto bus = std::make_shared<sdbusplus::asio::connection>(io);
    info("Start Interfaces Added");

    // Good match, match all under the '/'
    // e.g. /com
    // e.g. /com/bytedance
    // e.g. /com/bytedance/my_interface_path
    sdbusplus::bus::match_t goodMatch1(
        *bus, rules::interfacesAdded("/"),
        [](sdbusplus::message::message& msg) {
            sdbusplus::message::object_path p;
            msg.read(p);
            info("Good Match Full Path on path {PATH}", "PATH", std::string(p));
        });

    // Good match: Don't specify the path, match all
    // e.g. /com
    // e.g. /com/bytedance
    // e.g. /com/bytedance/my_interface_path
    sdbusplus::bus::match_t goodMatch2(
        *bus, rules::interfacesAdded(), [](sdbusplus::message::message& msg) {
            sdbusplus::message::object_path p;
            msg.read(p);
            info("Good Match No Path on path {PATH}", "PATH", std::string(p));
        });

    // Good match: Arg: If you want match some specific full path, you just
    // match all and specific the arg to Filter the incoming result. e.g.
    // e.g.: /com/bytedance (only)
    sdbusplus::bus::match_t goodMatch3(
        *bus, rules::interfacesAdded() + rules::argNpath(0, "/com/bytedance"),
        [](sdbusplus::message::message& msg) {
            sdbusplus::message::object_path p;
            msg.read(p);
            info("Good Match arg filter on path {PATH}", "PATH",
                 std::string(p));
        });

    // Good match: Arg namespace: all the path under the namespace
    // e.g.: /com/bytedance/my_interface_path
    // e.g.: /com/bytedance/xxx
    // /com/bytedance/xxx/xxx is bad
    sdbusplus::bus::match_t goodMatch4(
        *bus, rules::interfacesAdded() + rules::argNpath(0, "/com/bytedance/"),
        [](sdbusplus::message::message& msg) {
            sdbusplus::message::object_path p;
            msg.read(p);
            info("Good Match arg namespace on path {PATH}", "PATH",
                 std::string(p));
        });

    // Bad match
    sdbusplus::bus::match_t badMatch1(
        *bus, rules::interfacesAdded("/com/bytedance/"),
        [](sdbusplus::message::message& msg) {
            sdbusplus::message::object_path p;
            msg.read(p);
            info("Bad Match parent: on path {PATH}", "PATH", std::string(p));
        });

    io.run();
    return 0;
}
