#include <boost/asio.hpp>
#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/asio/connection.hpp>
#include <sdbusplus/asio/object_server.hpp>
#include <sdbusplus/asio/property.hpp>
#include <sdbusplus/bus/match.hpp>

PHOSPHOR_LOG2_USING;

// Assume you know the service name
constexpr auto service = "com.bytedance.my_test_aiso_service";
constexpr auto objectPath = "/com/bytedance/my_interface_path";
constexpr auto interface = "com.bytedance.my_interface";
constexpr auto propertyStr = "MyName";
constexpr auto propertyBool = "MyBoolType";
constexpr auto propertyInt = "MyIntType";
constexpr auto methodFoo = "foo";

namespace rules = sdbusplus::bus::match::rules;

using propertyVariantType = std::variant<std::string, bool, int>;
using propertyChangesType = std::map<std::string, propertyVariantType>;

int main(int /*argc*/, char** /*argv*/)
{
    boost::asio::io_context io;
    auto bus = std::make_shared<sdbusplus::asio::connection>(io);

    sdbusplus::bus::match_t propertyMatch(
        *bus, rules::propertiesChanged(objectPath, interface),
        [](sdbusplus::message::message& msg) {
            std::string interfaceName;
            propertyChangesType changedProperties;
            try
            {
                msg.read(interfaceName, changedProperties);
                info("propertiesChanged: interface: {INTERFACE}", "INTERFACE",
                     interfaceName);
                for (const auto& [p, v] : changedProperties)
                {
                    if (p == propertyStr)
                    {
                        info("Property {PROP} changed to {VALUE}", "PROP", p,
                             "VALUE", std::get<std::string>(v));
                    }
                    else if (p == propertyBool)
                    {
                        info("Property {PROP} changed to {VALUE}", "PROP", p,
                             "VALUE", std::get<bool>(v));
                    }
                    else if (p == propertyInt)
                    {
                        info("Property {PROP} changed to {VALUE}", "PROP", p,
                             "VALUE", std::get<int>(v));
                    }
                    else
                    {
                        error("Unknown property {PROP}", "PROP", p);
                    }
                }
            }
            catch (const std::exception& ex)
            {
                error("Failed to read msg, ex: {WHAT}", "WHAT", ex.what());
            }
        });
    sdbusplus::bus::match_t inetrfaceAddedMatch(
        *bus, rules::interfacesAdded(objectPath),
        [](sdbusplus::message::message& msg) {
            sdbusplus::message::object_path p;
            msg.read(p);
            info("Interface added on path {PATH}", "PATH", std::string(p));
        });
    sdbusplus::bus::match_t nameOwnerChangedMatch(
        *bus, rules::nameOwnerChanged(), [](sdbusplus::message::message& msg) {
            std::string name;
            std::string oldOwner;
            std::string newOwner;
            msg.read(name, oldOwner, newOwner);
            info("NameOwnerChanged, name {NAME}, old owner {OLDOWNER}, new "
                 "owner {NEWOWNER}",
                 "NAME", name, "OLDOWNER", oldOwner, "NEWOWNER", newOwner);
        });
    io.run();
    return 0;
}
