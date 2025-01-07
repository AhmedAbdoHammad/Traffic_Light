/********************************************************************************
**  FILENAME      : CONFIG.h                                                   **
**                                                                             **
**  VERSION       : 1.0.0                                                      **
**                                                                             **
**  DATE          : 2025-01-06                                                 **
**                                                                             **
**  PLATFORM      : ESP8266                                                    **
**                                                                             **
**  AUTHOR        : Ahmed Hammad                                               **
**                                                                             **
**  DESCRIPTION   : This file contains the program configurations              **
** 																			   **
**  Can Be Changed: Yes				                                           **
********************************************************************************/

#ifndef CONFIG.h
#define CONFIG.h
// Wi-Fi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// WebSocket server details
const char* websocket_server = "3.69.223.192";
const uint16_t websocket_port = 6001;

// Traffic light state machine
enum TrafficState { IDLE			  , 
					NORTH_SOUTH_GREEN , 
					NORTH_SOUTH_YELLOW, 
					EAST_WEST_GREEN   , 
					EAST_WEST_YELLOW  , 
					PED_CROSS		  , 
					EMERGENCY
					};
TrafficState currentState = IDLE;

// Pin definitions
const uint_8 NORTH_SOUTH_GREEN_PIN = 2;
const uint_8 NORTH_SOUTH_YELLOW_PIN = 3;
const uint_8 NORTH_SOUTH_RED_PIN = 4;
const uint_8 EAST_WEST_GREEN_PIN = 5;
const uint_8 EAST_WEST_YELLOW_PIN = 6;
const uint_8 EAST_WEST_RED_PIN = 7;
const uint_8 PED_NORTH_SOUTH_GREEN_PIN = 8;
const uint_8 PED_NORTH_SOUTH_RED_PIN = 9;
const uint_8 PED_EAST_WEST_GREEN_PIN = 10;
const uint_8 PED_EAST_WEST_RED_PIN = 11;
const uint_8 BUTTON_NORTH_SOUTH = 12;
const uint_8 BUTTON_EAST_WEST = 13;
const uint_8 EMERGENCY_BUTTON = 14;

// Shift Registers Pins for controlling seven sigments
const uint_8 DATA_PIN  = 15;  // DS (Data) pin
const uint_8 LATCH_PIN = 16;  // ST_CP (Latch) pin 
const uint_8 CLOCK_PIN = 17;  // SH_CP (Clock) pin 


// Number to Segment Mapping
const byte digitMap[] = {
  // gfedcba
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};


// Timing
const uint_32 GREEN_DURATION = 5000;  // 5 seconds
const uint_32 YELLOW_DURATION = 2000; // 2 seconds
const uint_32 RED_DURATION = 7000;    // 7 seconds
const uint_32 PED_DURATION = 7000;    // 7 seconds

#endif
