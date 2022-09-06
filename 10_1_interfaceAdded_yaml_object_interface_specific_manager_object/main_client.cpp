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

int main(int /*argc*/, char** /*argv*/)
{
    boost::asio::io_context io;
    auto bus = std::make_shared<sdbusplus::asio::connection>(io);
    info("Start Interfaces Added");

    // Good match: Path full match the manager_t, okay
    // e.g. /com/bytedance/my_interface_path
    // e.g. /com/bytedance/my_interface_path/sub1
    // e.g. /com/bytedance/my_interface_path/sub1/xxx
    sdbusplus::bus::match_t goodMatch1(
        *bus, rules::interfacesAdded("/com/bytedance/my_interface_path"),
        [](sdbusplus::message::message& msg) {
            sdbusplus::message::object_path p;
            msg.read(p);
            info("Good Match Full Path on path {PATH}", "PATH", std::string(p));
        });

    // Good match: Don't specify the path, match all
    // e.g. /com/bytedance
    // e.g. /com/bytedance/my_interface_path
    // e.g. /com/bytedance/my_interface_path/sub1
    sdbusplus::bus::match_t goodMatch2(
        *bus, rules::interfacesAdded(), [](sdbusplus::message::message& msg) {
            sdbusplus::message::object_path p;
            msg.read(p);
            info("Good Match No Path on path {PATH}", "PATH", std::string(p));
        });

    // Good match: Arg: If you want match some specific full path, you just
    // match all and specific the arg to Filter the incoming result.
    // e.g.: /com/bytedance/my_interface_path/sub1
    sdbusplus::bus::match_t goodMatch3(
        *bus,
        rules::interfacesAdded() +
            rules::argNpath(0, "/com/bytedance/my_interface_path/sub1"),
        [](sdbusplus::message::message& msg) {
            sdbusplus::message::object_path p;
            msg.read(p);
            info("Good Match arg filter on path {PATH}", "PATH",
                 std::string(p));
        });

    // Good match: Arg: If you want match some paths that under the specific
    // path.
    // e.g.: /com/bytedance/my_interface_path/sub1/aa
    // e.g.: /com/bytedance/my_interface_path/sub1/bb
    // /com/bytedance/my_interface_path/sub1/bb/cc is bad
    sdbusplus::bus::match_t goodMatch4(
        *bus,
        rules::interfacesAdded() +
            rules::argNpath(0, "/com/bytedance/my_interface_path/sub1/"),
        [](sdbusplus::message::message& msg) {
            sdbusplus::message::object_path p;
            msg.read(p);
            info("Good Match arg filter under the specific path {PATH}", "PATH",
                 std::string(p));
        });

    sdbusplus::bus::match_t badMatch1(*bus, rules::interfacesAdded("/"),
                                      [](sdbusplus::message::message& msg) {
                                          sdbusplus::message::object_path p;
                                          msg.read(p);
                                          info("Bad Match / on path {PATH}",
                                               "PATH", std::string(p));
                                      });

    // Bad match: specify the subpath `/com/bytedance/my_interface_path/sub1`,
    // but it's not a valid path
    sdbusplus::bus::match_t badMatch2(
        *bus, rules::interfacesAdded("/com/bytedance/my_interface_path/sub1"),
        [](sdbusplus::message::message& msg) {
            sdbusplus::message::object_path p;
            msg.read(p);
            info("Bad match sub1 on path {PATH}", "PATH", std::string(p));
        });

    // Bad match: specify the parent path `/com/bytedance/`, but it's not a
    // valid path
    sdbusplus::bus::match_t badMatch3(
        *bus, rules::interfacesAdded("/com/bytedance/"),
        [](sdbusplus::message::message& msg) {
            sdbusplus::message::object_path p;
            msg.read(p);
            info("Bad Match parent: on path {PATH}", "PATH", std::string(p));
        });

    io.run();
    return 0;
}
