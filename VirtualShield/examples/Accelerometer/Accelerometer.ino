#include <ArduinoJson.h>

#include <VirtualShield.h>
#include <Graphics.h>
#include <Accelerometer.h>

VirtualShield shield;			                    // identify the shield
Graphics screen = Graphics(shield);		            // connect a screen to the shield
Accelerometer accelermeter = Accelerometer(shield);         // connect an accelerometer to the shield

int startFastButtonId, stopButtonId, startTimedButtonId, startDeltaButtonId; // ids for start and stop buttons

// function to handle accelerometer events
void accelermeterEvent(ShieldEvent* event)
{
    screen.printAt(4, accelermeter.X);
    screen.printAt(5, accelermeter.Y);
    screen.printAt(6, accelermeter.Z);
}

// function to handle screen events
void screenEvent(ShieldEvent* event)
{
    if (screen.isButtonClicked(startFastButtonId))
    {
        accelermeter.start(); //report everything as fast as possible
    }
    
    if (screen.isButtonClicked(startTimedButtonId))
    {
        accelermeter.start(0, 1000); //only report every 1000 milliseconds (minimum)
    }
    
    if (screen.isButtonClicked(startDeltaButtonId))
    {
        accelermeter.start(0.2); //only report every 0.2 change in any reading (minimum)
    }

    if (screen.isButtonClicked(stopButtonId))
    {
        accelermeter.stop();
    }
}

// function to refresh (Refresh button, Connect) events
void refresh(ShieldEvent* event)
{
    screen.clear();	
    screen.print("Move the phone to see accelerometer readings");

    stopButtonId = screen.addButton(0, 200, "stop all");
    startFastButtonId = screen.addButton(0, 250, "start fast");
    startTimedButtonId = screen.addButton(0, 300, "start timed");
    startDeltaButtonId = screen.addButton(0, 350, "start delta");
}

void setup()
{
    screen.setOnEvent(screenEvent);
    shield.setOnRefresh(refresh);
    accelermeter.setOnEvent(accelermeterEvent);

    shield.begin(); // begin communication (automatically calls refresh event)
}

void loop()
{
    shield.checkSensors();
}
