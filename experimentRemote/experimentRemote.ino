
// Copyright (c) 2019 All Rights Reserved WestBot

#include <FastLED.h>
#include <Stepper.h>
#include <SoftwareSerial.h>

// USER DEFINE
#define DATA_PIN    D1
#define NUM_LEDS    14
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define RUNNING_FOR 30000L // 30 secondes
#define UPDATES_PER_SECOND 100

CRGB leds[ NUM_LEDS ];
CRGB color( 255, 0, 0 );

SoftwareSerial BTserial( D2, D3 );

const int stepsPerRevolution = 200; // change this to fit the number of steps per revolution
int stepCount = 0; // number of steps the motor has taken
//const int stepsPerRevolution = 2048; //2048 pour la 28BYJ-48 = 1 tour
int Nbre_tour = 0;

// initialize the stepper library on pins D5 to D8
Stepper myStepper( stepsPerRevolution, D6, D8, D7, D5 );

char c = ' ';

void setup() 
{
    myStepper.setSpeed( 100 ); // de 4 a 14 vitesse de rotation

    // Init LEDs
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
  
    for( int i = 0; i < NUM_LEDS; ++i )
    {
        leds[ i ] = CRGB::Black;
    }

    FastLED.show();
    
    // Configure the blinking led pin
    pinMode( LED_BUILTIN, OUTPUT );
 
    Serial.begin( 9600 );
    Serial.println("Welcome bitches");
 
    BTserial.begin( 9600 );  
    Serial.println( "BTserial started at 9600" );
    Serial.println( "Type '#' to enter configuration mode" );
}
 
void loop()
{
  /*
    myStepper.step( -100 );  
    for( int i = 0; i < NUM_LEDS; ++i )
    {
      leds[ i ] = CRGB::Red;
    }
    FastLED.show(); 
    delay( 50 );

    myStepper.step( -100 ); 
    for( int i = 0; i < NUM_LEDS; ++i )
    {
      leds[ i ] = CRGB::Black;
    }
    FastLED.show();   
    delay( 50 );
    */

    if( BTserial.available() )
    {  
        c = BTserial.read();
        Serial.write( c );

        // BEGIN EXPERIENCE
        if( c == 's' )
        {
            Serial.println( "Starting experience" );

            // Set PWM until reaching top
            for( uint32_t tStart = millis();  ( millis() - tStart ) < RUNNING_FOR; )
            {
                myStepper.step( -100 );  
                for( int i = 0; i < NUM_LEDS; ++i )
                {
                  leds[ i ] = color;
                }
                FastLED.show(); 
                delay( 50 );
                
                myStepper.step( -100 ); 
                for( int i = 0; i < NUM_LEDS; ++i )
                {
                  leds[ i ] = CRGB::Black;
                }
                FastLED.show();   
                delay( 50 );
            }

            while( 1 ) 
            {
              for( int i = 0; i < NUM_LEDS; ++i )
              {
                leds[ i ] = color;
              }
              FastLED.show(); 
              delay( 150 );
              
              for( int i = 0; i < NUM_LEDS; ++i )
              {
                leds[ i ] = CRGB::Black;
              }
              FastLED.show(); 
              delay( 150 );
            }
        }
        else if( c == '0' )
        {
            for( int i = 0; i < NUM_LEDS; ++i )
            {
                leds[ i ] = CRGB::Purple;
            }

            color = CRGB::Purple;
            FastLED.show();
        }
        else if( c == '1' )
        {
            for( int i = 0; i < NUM_LEDS; ++i )
            {
                leds[ i ] = CRGB::Yellow;
            } 

            color = CRGB::Yellow;
            FastLED.show();
        }
    }

    if( Serial.available() )
    {
        c =  Serial.read();
 
        if( c == '#' )  
        {
            Serial.print( "Entering configuration mode (type '&' to exit)" );
        }
        else if( c == '$' ) 
        {
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
