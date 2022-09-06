
Run the match app
```
./build/10_dbus_matches/test_client_match
```

Run the asio server in another terminal
```
./build/06_dbus_asio_service_with_methods/test_asio
```

Now see the matched callbacks
```
<6> NameOwnerChanged, name :1.5576, old owner , new owner :1.5576
<6> propertiesChanged: interface: com.bytedance.my_interface
<6> Property MyName changed to foo
<6> propertiesChanged: interface: com.bytedance.my_interface
<6> Property MyBoolType changed to True
<6> propertiesChanged: interface: com.bytedance.my_interface
<6> Property MyIntType changed to 42
<6> NameOwnerChanged, name com.bytedance.my_test_aiso_service, old owner , new owner :1.5576
```

Kill the asio server in another terminal, and see the callbacks:
```
<6> NameOwnerChanged, name com.bytedance.my_test_aiso_service, old owner :1.5576, new owner
<6> NameOwnerChanged, name :1.5576, old owner :1.5576, new owner
```

Note the `interfacesAdded` signal is not received because the server does not emit such signals.
