arduino_device_interface
------------------------

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

##Example Usage

###Arduino Firmware

[Arduino Sketch Examples](./Examples)

###Host Computer Interface

For more detailed host computer interface information, see example
sketch README files.

[LedController](./Example/LedController)

####Arduino Serial Monitor

Open the Serial Monitor in the Arduino IDE.

Set the baud rate to match the value in your Arduino sketch.

Set the line ending to 'Newline'.

To get help information about the Arduino device, type a single
question mark ? into the input field and press the 'Send' button or
press the 'Enter' key.

```shell
?
```

The response will contain a field named "commands", which is an array
of user commands. To execute a command, simply type it into the input
field and press the 'Send' button or press the 'Enter' key.

####Python

<https://github.com/JaneliaSciComp/python_arduino_device>

####Matlab

<https://github.com/JaneliaSciComp/matlab_arduino_device>

##Installation

This library and its dependencies can be installed together or
separately.

###Install This Library and its Dependencies Together

Clone the arduino-libraries repository:

```shell
git clone https://github.com/JaneliaSciComp/arduino-libraries.git
```

Install symbolic library links into the default sketchbook directory
using Python when running Linux or Mac OS X:

```shell
python symlinks.py --install
```
Or for more information on installing libraries, see:

<http://arduino.cc/en/Guide/Libraries>

###Install This Library and its Dependencies Separately

####Clone This Repository into the Directory 'DeviceInterface'

```shell
git clone https://github.com/JaneliaSciComp/arduino_device_interface.git DeviceInterface
```

Install symbolic library link into the default sketchbook directory
using Python when running Linux or Mac OS X:

```shell
python symlink.py --install
```
Or for more information on installing libraries, see:

<http://arduino.cc/en/Guide/Libraries>

####Download and Install the Arduino Library Dependencies

Streaming:

<http://arduiniana.org/libraries/streaming/>

ArduinoJson:

<https://github.com/bblanchon/ArduinoJson>

StandardCplusplus:

<https://github.com/maniacbug/StandardCplusplus>


