
IoT Tracking Server is a multi threaded server for tracking devices "phoning home".  Its purpose is to track...

* the number of devices in the field
* each device's model number
* each device's version
* each device's time since last boot and number of boots

Other metrics can easily be added by modifying the Record and Client classes.


### Features

* GUI for remote live monitoring (coming soon)
* Compile time support for SSL encryption.
* Compile time support for SSL client authentication
* Compile time support for number of worker threads
* Compile time support for listening port number.

Compile time options are listed in global.h

### Usage on IoT device

Some sudo code on the IoT device to phone home....

```c++
TCPSocket socket;
socket.connectToHost(someIPaddress, 12345);
socket.write("id=334586\nmodel=K9000\nversion=1.00\nboot\n");
socket.waitForBytesWritten();
socket.close();
```
The metrics must be new line separated.  The unique ID must be sent first.  The "Action" metrics (boot or 24hrs) must be sent last.  The "Action" metrics trigger updating the record.

### Benchmarks / Performance

Aprox 0.25% core load per connection on a Intel Xeon E5-1650 v4 running at 1.2Ghz. or 400 connections per second = 100% core usage.  Theoretically the max clock on my cpu is 3.6Ghz, so I could get 1200 connections per second...and I have 12 cores, so maybe even lots more not taking into consideration cache and memory bus speed.

### Memory Usage

Aprox 30MByte per 100,000 records (devices).  This will increase when you add more metrics to the Record class.

### SSL Certificates

A [how too guide](http://www.infidigm.net/articles/qsslsocket_for_ssl_beginners/) for generating SSL certificates to use with the IoT Tracker Server.

### Dependencies

* Qt5
* [QZeroConf](https://github.com/jbagg/QtZeroConf) if using the remote live monitoring GUI.

### Licensing

IoT Tracker Server has a modified BSD license.  It has an extra clause added that states that the IoT Tracker Server software can not be used to track personal or identifying information.