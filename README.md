

#Windows Virtual Shields for Arduino (Library)

This project lets your Arduino sketch control your Windows 10 device (phone). The only code you write is to an Arduino sketch.

This repository is an Arduino library that allows the Arduino to connect over Bluetooth to the [Virtual Shields Universal Windows Application](https://github.com/ms-iot/virtual-shields-universal).

Windows Virtual Shields for Arduino is an open-source library primarily for the Arduino UNO which communicates with an open-source Windows Universal Application
running on all Windows 10 devices, primarily focusing on Windows Phones.  The library exposes the Phones' sensors and capabilities to the an Arduino Wiring Sketch.
The following sensors and capabilities can be used from Arduino Wiring:

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

* Place buttons on your Windows Phone's screen which directly affect pins, motors on your Arduino.
  * Shields used:
     * Screen 
* Send data gathered on the Arduino (Weather Station) to the cloud through the phone's data connection.
* or check a web page for a weather status and open or close windows in your house. 
  * Shields used:
     * GPS
     * Web 
* Turn the phone's accelerometer into a remote driving controller for an Arduino robot.
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
 3. Windows 10 phone ([Lumia 520](http://www.microsoft.com/en-us/mobile/phone/lumia520/), [Lumia 635](http://www.microsoft.com/en-us/mobile/phone/lumia635/))

###Set up your Arduino
 1. Prepare the Bluetooth module if necessary (the Bluetooth module may need to have headers soldered onto it).
 2. Except for one difference below, connect the Bluetooth module to the Arduino per your wiring diagram ([BlueSMiRF wiring diagram](https://learn.sparkfun.com/tutorials/using-the-bluesmirf/hardware-hookup)).
    
    DIFFERENCE: Use pins 0 and 1 instead of 2 and 3:  
        The Bluetooth TX should connect to pin 0 (Arduino RX).  
        The Bluetooth RX should connect to pin 1 (Arduino TX).  

###Set up your Windows 10 phone ( [using the virtual-shields-universal repository](https://github.com/ms-iot/virtual-shields-universal))
 1. Windows 8 users can get the "Windows Insider" application from the app store. This allows the user to opt into receiving Windows 10 Technical Previews as updates.
 2. Pair the Bluetooth device in the Bluetooth settings.  
    BlueSMiRF default pin code is 1234.  
    NOTE: The red blinking light on the BlueSMiRF continues to blink red after a successful pairing. This is expected. It only turns green after a connecting with the application.  
    When pairing you should see a device name like "RNBT-76BC" the last four characters are the last four characters of the MAC address printed on the sticker of the module.  

##Getting Started : Software

###What you need
 1. Arduino IDE 1.6 or better.
 2. ArduinoJson library.
 3. This repository.
 4. Visual Studio 2015 to sideload UWA (phone app) onto developer unlocked phone.

###Set up your Arduino IDE
 1. Download and install the [Arduino IDE](http://www.arduino.cc/en/Main/Software).

###Set up ArduinoJson library
 1. From the [ArduinoJson repository](https://github.com/bblanchon/ArduinoJson), branch the reporsitory or download the zip.
 2. Place the whole repository into your libraries folder (i.e. Documents\Arduino\libraries\ArduinoJson\).

###Set up this repository.
 1. Branch this repository or download the zip.
 2.	Copy this repository directly to your Arduino library (i.e. Documents\Arduino\libraries\).

###Set up your Visual Studio 2015 [using the virtual-shields-universal repository](https://github.com/ms-iot/virtual-shields-universal)
 1. Get the Windows 10 Technical Preview tools, including Visual Studio 2015 from [dev.windows.com](https://dev.windows.com/en-us/windows-10-developer-preview-tools).
 2. Load the Shield.sln from this repository (<repository>/Shield/Shield.sln).
 3. Ensure your phone is developer-unlocked [run the Windows Phone Developer Registration tool installed with the tools](https://msdn.microsoft.com/en-us/library/windows/apps/dn614128.aspx). 
 4. Deploy to your device.
 5. Run the Virtual Shields for Arduino application.
 6. In the app settings, find your previously paired Bluetooth device and Connect.

###Test your setup
 1. From the Arduino IDE, go to the menu item File->Examples->Virtual Shields->Hello Blinky. This should load the Hello Blinky example.
 2. Before downloading, temporarily remove the Bluetooth TX and RX wires from the Arduino. (There is only one serial port shared between the USB and Bluetooth. The Bluetooth interferes with the download).
 3. Download the sketch.
 4. Replace the Bluetooth TX and RX wires into the Arduino pins. (Bluetooth TX to Arduino RX and Bluetooth RX to Arduino TX).
 5. Press the Reset button on the Arduino to restart the sketch.
 6. (You should see a message on your phone and be able to interact).

##Examples included in this package:
 1. Print Hello World on your phone (Screen text).
 2. Using buttons on your phone to control pins (Screen graphics).
 3. Create a Weather Forecast Indicator (GPS, Web).
 4. Create a math-quiz door lock (open Speech Recognition).
 5. Voice-controlled LED light (SRGS-constrained Speech Recognition)

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
