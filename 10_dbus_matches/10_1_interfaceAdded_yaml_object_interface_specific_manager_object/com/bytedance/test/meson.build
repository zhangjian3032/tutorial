test_hpp = custom_target(
    'server.hpp',
    capture: true,
    command: [
        sdbuspp,
        '-r', meson.source_root() / '10_dbus_matches/10_1_interfaceAdded_yaml_object_interface_specific_manager_object',
        'interface',
        'server-header',
        'com.bytedance.test',
    ],
    input: '../test.interface.yaml',
    install: true,
    install_dir: get_option('includedir') / 'com/bytedance/test',
    output: 'server.hpp',
)

test_cpp = custom_target(
    'server.cpp',
    capture: true,
    command: [
        sdbuspp,
        '-r', meson.source_root() / '10_dbus_matches/10_1_interfaceAdded_yaml_object_interface_specific_manager_object',
        'interface',
        'server-cpp',
        'com.bytedance.test',
    ],
    input: '../test.interface.yaml',
    output: 'server.cpp',
)
