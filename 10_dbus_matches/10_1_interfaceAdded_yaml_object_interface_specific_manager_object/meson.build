# use to create a test interface
subdir('com/bytedance/test')

executable(
  'test_server_match',
  'main_server.cpp',
  test_cpp,
  test_hpp,
  dependencies: [
    sdbusplus_dep,
    phosphor_logging_dep,
  ])

executable(
  'test_client_match',
  'main_client.cpp',
  dependencies: [
    sdbusplus_dep,
    phosphor_logging_dep,
  ])