
#include "Sms.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

const PROGMEM char SERVICE_SMS[] = "SMS";
const PROGMEM char SUBJECT[] = "Subject";
const PROGMEM char CC[] = "Cc";

/// <summary>
/// Initializes a new instance of the <see cref="Sms"/> class.
/// </summary>
/// <param name="shield">The shield.</param>
Sms::Sms(const VirtualShield &shield) : Sensor(shield, 'Y') {
}

/// <summary>
/// Initiates an SMS to the specified number.
/// </summary>
/// <param name="to">The destination number</param>
/// <param name="message">The message.</param>
/// <param name="attachment">The attachment url.</param>
/// <returns>int.</returns>
int Sms::send(String to, String message, String attachment) {
	EPtr eptrs[] = { EPtr(MemPtr, TO, to.c_str()), EPtr(MemPtr, MESSAGE, message.c_str()), EPtr(attachment ? MemPtr : None, ATTACHMENT, attachment.c_str()) };
	return shield.writeAll(SERVICE_SMS, eptrs, 3);
}

/// <summary>
/// Event called when a valid json message was received. 
/// Consumes the proper values for this sensor.
/// </summary>
/// <param name="root">The root json object.</param>
/// <param name="shieldEvent">The shield event.</param>
void Sms::onJsonReceived(JsonObject& root, ShieldEvent* shieldEvent) {
	Sensor::onJsonReceived(root, shieldEvent);
}
