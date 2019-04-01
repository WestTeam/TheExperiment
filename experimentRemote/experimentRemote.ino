
// Copyright (c) 2019 All Rights Reserved WestBot

#include <SoftwareSerial.h>

// USER DEFINE
#define enA 9
#define in1 6
#define in2 7
#define ATmodePin 4
#define LedPin 5
#define PwmOutput 255 // FULL SPEED NOW

#define RUNNING_FOR 30000L // 30 secondes

SoftwareSerial BTserial( 2, 3 );
 
char c = ' ';
 
void setup() 
{
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
              digitalWrite( LedPin, HIGH );
              digitalWrite( LED_BUILTIN, HIGH );   // turn the LED on (HIGH is the voltage level)
              delay( 1000 );
              digitalWrite( LedPin, LOW );
              digitalWrite( LED_BUILTIN, LOW );   // turn the LED on (HIGH is the voltage level)
              delay( 1000 );               
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
