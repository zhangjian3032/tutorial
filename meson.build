project('openbmc-tutorials', 'cpp',
    default_options: [
      'buildtype=debugoptimized',
      'cpp_std=c++20',
      'warning_level=3',
      'werror=true',
    ],
    version: '1.0.0',
    meson_version: '>=0.57.0',
)

libsystemd_pkg = dependency('libsystemd')
sdbusplus_dep = dependency('sdbusplus')
phosphor_logging_dep = dependency('phosphor-logging')
dbus_dep = dependency('phosphor-dbus-interfaces')
sdbuspp = find_program('sdbus++')

subdir('10_dbus_matches')
