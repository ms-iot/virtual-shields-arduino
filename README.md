

#Windows Virtual Shields for Arduino (Library)

This project lets an Arduino sketch control a device running Windows 10 - be it a PC, a Windows Phone, or even a Raspberry Pi 2. The only code you write is to an Arduino sketch.

The Windows Virtual Shields for Arduino repository contains an Arduino library that allows an Arduino to connect over Bluetooth to the [Virtual Shields Universal Windows Application](https://github.com/ms-iot/virtual-shields-universal).  This open-source library was written primarily for Arduino UNO, and it can be used alongside any Windows 10 device running the Universal Windows Application linked above.

The power of this application is best exemplified when run on the Windows Phone - the library exposes most of the Phone's sensors and capabilities (or shields, if we're using Arduino terminology) to an Arduino Wiring Sketch.

The following sensors and capabilities can be controlled directly from Arduino Wiring:

Sensors:
* Accelerometer
* Compass
* Geolocator (GPS)
* Gyrometer
* Light Sensor
* Orientation
 
Capabilities:
* Camera
* Device info (name, date/time/timezone, os)
* Email (initiation)
* Microphone
* Notifications (Tile/Toast)
* Screen (Text, Images, Audio/Video, Rectangles, Buttons, Touchscreen)
* Sms (initiation)
* Speech to Text and Speech Recognition
  * can receive table data from previous web search 
* Vibration
* Web (Get and Post with result parsing)
  * XPath
  * JSON
  * Simple text
  * Regular expressions
  * Table iteration (keys/values saved)

##Within your Arduino Wiring Sketch, you can enable the following scenarios:

* Place buttons on your device's screen which directly affect pins, motors on your Arduino.
  * Shields used:
     * Screen 
* Send data gathered on the Arduino (Weather Station) to the cloud through the device's data connection.
* or check a web page for a weather status and open or close windows in your house. 
  * Shields used:
     * GPS
     * Web 
* Turn the accelerometer on a Windows Phone into a remote driving controller for an Arduino robot.
  * Shields used:
     * Accelerometer 
* Ask questions and based on voice responses, control pins to turn on lights or motors.
  * Shields used:
     * Text to Speech and Speech Recognition. 
* Security Cam: Sense movement with an Arduino ultrasonic sensor, take a picture, record audio, upload to Azure and alert you.
  * Shields used:
     * Web
     * Camera
     * Microphone 

##Getting Started : Hardware

###What you need
 1. Arduino Uno or compatible device.
 2. Bluetooth module: SparkFun BlueSMiRF Silver (https://www.sparkfun.com/products/12577) and 4 wires to connect.
 3. A Windows 10 device: because of its many "shields", we recommend a Windows Phone ([Lumia 520](http://www.microsoft.com/en-us/mobile/phone/lumia520/), [Lumia 635](http://www.microsoft.com/en-us/mobile/phone/lumia635/))

###Set up your Arduino
 1. Prepare the Bluetooth module if necessary (the Bluetooth module may need to have headers soldered onto it).
 2. Except for one difference below, connect the Bluetooth module to the Arduino per your wiring diagram ([BlueSMiRF wiring diagram](https://learn.sparkfun.com/tutorials/using-the-bluesmirf/hardware-hookup)).
    
    DIFFERENCE: Use pins 0 and 1 instead of 2 and 3:  
        The Bluetooth TX should connect to pin 0 (Arduino RX).  
        The Bluetooth RX should connect to pin 1 (Arduino TX).  

###Set up your Windows 10 device ( [using the virtual-shields-universal repository](https://github.com/ms-iot/virtual-shields-universal))
 1. For Windows Phone: Windows 8 users can get the "Windows Insider" application from the app store. This allows the user to opt into receiving Windows 10 Technical Previews as updates.
 2. For other devices: make sure you're running Windows 10.
 3. Pair your device to the Arduino Bluetooth device in the Bluetooth settings.  
    BlueSMiRF default pin code is 1234.  
    NOTE: The red blinking light on the BlueSMiRF continues to blink red after a successful pairing. This is expected. It only turns green after a connecting with the application.  
    When pairing you should see a device name like "RNBT-76BC" the last four characters are the last four characters of the MAC address printed on the sticker of the module.  

##Getting Started : Software

###What you need
 1. Arduino IDE 1.6 or better.
 2. ArduinoJson library.
 3. This repository.
 4. (OPTIONAL) Visual Studio 2015 to sideload UWA onto developer unlocked device.

###Set up your Arduino IDE
 1. Download and install the [Arduino IDE](http://www.arduino.cc/en/Main/Software).

###Set up ArduinoJson library
 1. From the [ArduinoJson repository](https://github.com/bblanchon/ArduinoJson), branch the reporsitory or download the zip.
 2. Place the whole repository into your libraries folder (i.e. Documents\Arduino\libraries\ArduinoJson\).

###Set up this repository.
 1. Branch this repository or download the zip.
 2.	Copy this repository directly to your Arduino library (i.e. Documents\Arduino\libraries\).

###Run the Virtual Shields for Arduino app on your device.
There are two ways to get the app running on your device, outlined below.

####Download the app from the Windows Store.
1. If your device has the capability to access the Windows Store, simply download the "Virtual Shields for Arduino" application directly.  The app can be found [here](https://www.microsoft.com/store/apps/9nblgggz0mld).

####Deploy the app with Visual Studio 2015 [using the virtual-shields-universal repository](https://github.com/ms-iot/virtual-shields-universal)
 1. Get the Windows 10 Technical Preview tools, including Visual Studio 2015 from [dev.windows.com](https://dev.windows.com/en-us/windows-10-developer-preview-tools).
 2. Load the Shield.sln from this repository (<repository>/Shield/Shield.sln).
 3. Ensure your device is developer-unlocked: on phone, [run the Windows Phone Developer Registration tool installed with the tools](https://msdn.microsoft.com/en-us/library/windows/apps/dn614128.aspx). 
 4. Deploy the solution to your device.
 5. Run the newly installed Virtual Shields for Arduino application.
 6. In the app settings, find your previously paired Bluetooth device and Connect.

###Test your setup
 1. From the Arduino IDE, go to the menu item File->Examples->Virtual Shields->Hello Blinky. This should load the Hello Blinky example.
 2. Before downloading, temporarily remove the Bluetooth TX and RX wires from the Arduino. (There is only one serial port shared between the USB and Bluetooth. The Bluetooth interferes with the download).
 3. Download the sketch.
 4. Replace the Bluetooth TX and RX wires into the Arduino pins. (Bluetooth TX to Arduino RX and Bluetooth RX to Arduino TX).
 5. Press the Reset button on the Arduino to restart the sketch.
 6. You should see a message on your device and be able to interact.

##Examples included in this package:
 1. Print Hello World on your device (Screen text, multiple devices).
 2. Using buttons on your device to control pins (Screen graphics, multiple devices).
 3. Create a Weather Forecast Indicator (GPS and Web, Windows Phone).
 4. Create a math-quiz door lock (open Speech Recognition, multiple devices).
 5. Voice-controlled LED light (SRGS-constrained Speech Recognition, multiple devices)

##Troubleshooting:
*	Did you build a custom sketch? Remember to include “ shield.begin() “ in your setup():

```c++
      shield.begin();      // for default 115200 baud rate
```

*	If nothing shows up, some devices require a baud of 57600, and even some at 9600. Try these in your sketch “ shield.begin(57600) “ in your setup().

```c++
      shield.begin(57600); // for 57600 baud rate
```

*	Double check that your Bluetooth’s TX pin is connected to your Arduino’s RX pin and that the Bluetooth’s RX pin is connected to your Arduino’s TX pin per this [diagram](https://learn.sparkfun.com/tutorials/using-the-bluesmirf/hardware-hookup).
*	Is there another strong broadcasting Bluetooth device around, like headphones or a Bluetooth mouse? This can interfere.

##Arduino Wiring Sketch : Hello World example

```c++

    #include <ArduinoJson.h>

    #include <VirtualShield.h>
    #include <Text.h>

    VirtualShield shield;        // identify the shield
    Text screen = Text(shield);  // connect the screen

    void setup()
    {
	    shield.begin();              // begin communication

	    screen.clear();              // clear the screen
	    screen.print("Hello Windows Virtual Shields for Arduino");	 
    }

    void loop()
    {
    }
```
