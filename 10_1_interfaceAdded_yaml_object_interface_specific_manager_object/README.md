
Run the match app
```
# a client app to wait for the signal
./build/10_dbus_matches/10_1_interfaceAdded_yaml_object_interface_specific_manager_object/test_client_match

```

Run the asio server in another terminal
```
# a server app to emit the signal
./build/10_dbus_matches/10_1_interfaceAdded_yaml_object_interface_specific_manager_object/test_server_match
```

Now see the matched callbacks, details are in the comments of the code
```
<6> Good Match No Path on path /com/bytedance/my_interface_path
<6> Good Match Full Path on path /com/bytedance/my_interface_path
<6> Good Match arg filter on path /com/bytedance/my_interface_path/sub1
<6> Good Match No Path on path /com/bytedance/my_interface_path/sub1
<6> Good Match Full Path on path /com/bytedance/my_interface_path/sub1
<6> Good Match arg filter under the specific path /com/bytedance/my_interface_path/sub1/subsub1
<6> Good Match No Path on path /com/bytedance/my_interface_path/sub1/subsub1
<6> Good Match Full Path on path /com/bytedance/my_interface_path/sub1/subsub1

```