#include <Arduino.h>
#include <Wire.h>
#include "AS5601.h"

static constexpr uint32_t I2C_CLOCK_HZ = 100000;
// Side + 3-digit index: "L###" for the left gripper, "R###" for the right,
// e.g. "L003" / "R003".
static const char* DEVICE_ID = "CHANGE HERE";


AS5601 encoder(Wire, 0x36, false);

void setup()
{
	Serial.begin(115200);
	while (!Serial)
	{
		delay(10);
	}

#if defined(ESP32) || defined(ARDUINO_ARCH_ESP32)
	constexpr int SDA_PIN = 20;
	constexpr int SCL_PIN = 19;
	encoder.begin(I2C_CLOCK_HZ, SDA_PIN, SCL_PIN);
#else
	encoder.begin(I2C_CLOCK_HZ);
#endif

	//encoder.setZeroPosition(encoder.getRawAngle());

	Serial.print("# AS5601 streaming: rad, device_id=");
	Serial.println(DEVICE_ID);

}

void loop()
{

	double radians = encoder.getAngleRadians();
	Serial.print(DEVICE_ID);
	Serial.print(",");
	Serial.println(radians, 8);

	const String diag = encoder.getDiagnostic();
	if (diag.length() > 0 && diag != "Magnetic field OK")
	{
		Serial.println(diag);
	}

	delay(5);

}