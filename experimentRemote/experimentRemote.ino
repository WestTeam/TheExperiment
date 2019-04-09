
// Copyright (c) 2019 All Rights Reserved WestBot

#include <FastLED.h>
#include <SoftwareSerial.h>

// USER DEFINE
#define LED_PIN     5
#define NUM_LEDS    14
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define enA 9
#define in1 6
#define in2 7
#define ATmodePin 4
#define LedPin 5
#define PwmOutput 255 // FULL SPEED NOW

#define RUNNING_FOR 30000L // 30 secondes
#define UPDATES_PER_SECOND 100

CRGB leds[NUM_LEDS];
const CRGB lightcolor( 255, 128, 128 );

SoftwareSerial BTserial( 2, 3 );
 
char c = ' ';
 
void setup() 
{
    // Init LEDs
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );

    // Static red for QR
    leds[0] = lightcolor; 
    leds[1] = lightcolor; 
    leds[2] = lightcolor; 
    leds[3] = lightcolor; 
    leds[4] = lightcolor; 
    FastLED.show();

    // Configure pin for PWM and motor control
    pinMode( enA, OUTPUT );
    pinMode( in1, OUTPUT );
    pinMode( in2, OUTPUT );
    
    // Set initial rotation direction
    digitalWrite( in1, LOW );
    digitalWrite( in2, HIGH );
  
    // Configure the blinking led pin
    pinMode( LedPin, OUTPUT ); 
    pinMode(LED_BUILTIN, OUTPUT);

    // Configure pin to enter AT mode
    pinMode( ATmodePin, OUTPUT ); 
    digitalWrite( ATmodePin, LOW ); 
 
    Serial.begin( 9600 );
    Serial.println("Welcome bitches");
 
    BTserial.begin( 9600 );  
    Serial.println( "BTserial started at 9600" );
    Serial.println( "Type '#' to enter configuration mode" );
}
 
void loop()
{
    if( BTserial.available() )
    {  
        c = BTserial.read();
        Serial.write( c );

        // BEGIN EXPERIENCE
        if( c == 's' )
        {
            Serial.println( "Starting experience" );

            analogWrite( enA, PwmOutput ); // Send PWM signal to L298N Enable pin

            // Set PWM until reaching top
            for( uint32_t tStart = millis();  ( millis() - tStart ) < RUNNING_FOR; )
            {
              leds[0] = CRGB::Red; 
              leds[1] = CRGB::Red;
              leds[2] = CRGB::Red;
              leds[3] = CRGB::Red;
              leds[4] = CRGB::Red;
              FastLED.show();
              delay( 500 );
              leds[0] = CRGB::Black; 
              leds[1] = CRGB::Black;
              leds[2] = CRGB::Black;
              leds[3] = CRGB::Black;
              leds[4] = CRGB::Black;
              FastLED.show();
              delay( 500 );              
            }

            analogWrite( enA, 0 ); // STOP PWM
            // END EXPERIENCE
        }
    }

    if( Serial.available() )
    {
        c =  Serial.read();
 
        if( c=='#' )  
        {
            digitalWrite( ATmodePin, HIGH );
            Serial.print( "Entering configuration mode (type '&' to exit)" );
        }
        else if( c == '$' ) 
        {
            digitalWrite( ATmodePin, LOW );
            BTserial.print( "AT+RESET\n\r" );
            Serial.print( "AT+RESET\n\r" ); 
        }        
        else
        {
              Serial.write( c );
              BTserial.write( c );  
        }
    }
}
