/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x32 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <Adafruit_Microbit.h>

#define buttonA 5     // the number of the pushbutton pin
#define buttonB 11     // the number of the pushbutton pin

#define INT_TEMP_SENSOR 0

#define INT_RELAY_TRIGGER 1 

Adafruit_Microbit_Matrix microbit;

void setup()
{
    Serial.begin(115200);

    microbit.begin();

    pinMode(buttonA, INPUT);  
    pinMode(buttonB, INPUT);

    pinMode( INT_TEMP_SENSOR, INPUT );
    pinMode( INT_RELAY_TRIGGER, OUTPUT );
}


uint32_t tick = 1;
uint32_t output = 0;
uint32_t button_press = 0;


void loop() 
{
    if (! digitalRead(buttonA)) 
    {
        Serial.println("Button A pressed");

        button_press = 1;
    }

    if (! digitalRead(buttonB)) 
    {
        Serial.println("Button B pressed");

        button_press = 0;
    }

    if( ( tick++ % 10 ) == 0 )
    {
        uint32_t celsius = analogRead( INT_TEMP_SENSOR ) * 300 / 1023;

        output = (int) ( celsius / 33 ) | button_press;

        microbit.println( (int) output );

        Serial.println( celsius );

        microbit.print( (int) celsius );

        digitalWrite( INT_RELAY_TRIGGER, output );
    }

    delay( 200 );
}
