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

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

void drawString( const char* _string, uint32_t x = 5, uint32_t y = 5 );


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define buttonA 5     // the number of the pushbutton pin
#define buttonB 11     // the number of the pushbutton pin

#define INT_TEMP_SENSOR 0

// pimoroni automation:bit
// these are hardwired so be careful when combining other microbit accessories

#define INT_RELAY_TRIGGER 16 // hardwired for pimoroni automation bit
#define INT_DIGITAL_OUT_ONE 14
#define INT_DIGITAL_OUT_TWO 15
#define INT_DIGITAL_IN_ONE 8
#define INT_DIGITAL_IN_TWO 13
#define INT_ANALOG_ONE 2
#define INT_ANALOG_TWO 1
#define INT_ANALOG_THREE 0


// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display( SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET );

void setup()
{
    Serial.begin(115200);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin( SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS ) ) 
    {
        Serial.println(F("SSD1306 allocation failed"));
        return;
    }

    pinMode(buttonA, INPUT);  
    pinMode(buttonB, INPUT);
    pinMode( INT_TEMP_SENSOR, INPUT );
    pinMode( INT_RELAY_TRIGGER, OUTPUT );

    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.display();
    delay(200); // Pause for 0.2 seconds

    // Clear the buffer
    display.clearDisplay();

    // Show the display buffer on the screen. You MUST call display() after
    // drawing commands to make them visible on screen!
    
    delay( 200 );

    // display.display() is NOT necessary after every single drawing command,
    // unless that's what you want...rather, you can batch up a bunch of
    // drawing operations and then update the screen all at once by calling
    // display.display(). These examples demonstrate both approaches...

    drawString( "hello world!" );
}

char  tempString[ 4 ] = "";
uint32_t tick = 1;
uint32_t output = 0;

void loop() 
{
    if (! digitalRead(buttonA)) 
    {
        Serial.println("Button A pressed");

        output = 1;
    }

    if (! digitalRead(buttonB)) 
    {
        Serial.println("Button B pressed");

        output = 0;
    }

    if( ( tick++ % 10 ) == 0 )
    {
        display.clearDisplay();

        uint32_t celsius = analogRead( INT_TEMP_SENSOR ) * 300 / 1023;

        output = (int) ( celsius / 33 );

        Serial.println( celsius );

        itoa( celsius, tempString, 10 );

        drawString( tempString );

        itoa( output, tempString, 10 );

        drawString( tempString, 64, 5 );

        digitalWrite( INT_RELAY_TRIGGER, output );
    }

    delay( 200 );
}


void drawString( const char* _string, uint32_t x, uint32_t y ) 
{
    Serial.println( "drawString" );
    Serial.println( _string );

    int16_t x1, y1;
    uint16_t w, h;

    // display.clearDisplay();
    display.getTextBounds( _string, (int16_t) x, (int16_t) y, &x1, &y1, &w, &h );
    display.fillRect( x1, y1, w, h, SSD1306_BLACK );

    display.setTextSize( 2 );
    display.setTextColor( SSD1306_WHITE ); // Draw white text
    display.setCursor( x, y );             // Start at top-left corner
    display.cp437( false ); // Use full 256 char 'Code Page 437' font

    // Not all the characters will fit on the display. This is normal.
    // Library will draw what it can and the rest will be clipped.

    for( size_t i = 0; i < strlen( _string ); i++)
    {
        display.write( _string[ i ] );
    }

    display.display();
}
