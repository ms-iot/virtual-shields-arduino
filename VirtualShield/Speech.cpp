#include "Speech.h"
#include "Sensor.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

const PROGMEM char SERVICE_NAME_SPEECH[] = "SPEECH";
const int MEDIA_PAUSED = 4;

/// <summary>
/// Initializes a new instance of the <see cref="Speech"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Speech::Speech(const VirtualShield &shield) : Sensor(shield, 'T')
{
}

/// <summary>
/// Speaks the specified message.
/// </summary>
/// <param name="message">The message.</param>
/// <returns>The id of the message. Negative if an error.</returns>
int Speech::speak(String message)
{
    IsSpeaking = true;
	EPtr eptrs[] = { EPtr(MemPtr, MESSAGE, message.c_str()) };
    return shield.block(writeAll(SERVICE_NAME_SPEECH, eptrs, 1), onEvent == 0, WAITFOR_TIMEOUT, MEDIA_PAUSED);
}

int Speech::stop()
{
	return Sensor::sendStop(SERVICE_NAME_SPEECH);
    IsSpeaking = false;
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Speech::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) 
{
    if (shieldEvent->resultId < 1 || shieldEvent->resultId > 3) {
        IsSpeaking = false;
    }
    else if (shieldEvent->resultId > 0 && shieldEvent->resultId < 4) {
        IsSpeaking = true;
    }

	Sensor::onJsonReceived(root, shieldEvent);
}
