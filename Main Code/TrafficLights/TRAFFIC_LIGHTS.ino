/*****************************************************************************************************
**  FILENAME      : TrafficLights.ino                                                               **
**                                                                                                  **
**  VERSION       : 1.0.0                                                                           **
**                                                                                                  **
**  DATE          : 2025-01-05                                                                      **
**                                                                                                  **
**  PLATFORM      : ESP8266                                                                         **
**                                                                                                  **
**  AUTHOR        : Ahmed Hammad                                                                    **
**                                                                                                  **
**  DESCRIPTION   : This system controls traffic lights and pedestrian signalsusing an FSM          **
*                  (Finite State Machine) approach. The code is implemented for an Arduino ESP8266, **
*                  managing lights and signals based on predefined timers and pedestrian button     **
*                  presses. It ensures safety and prevents conflicting traffic flows.               **
*   Can Be Changed: No (Static Code)                                                                **
******************************************************************************************************/

/*****************************************************************************************************
**                                     Include Librarie                                             **
*****************************************************************************************************/
#include "STD_TYPES.h"
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include "CONFIG.h"
#include "INTERFACE.h"

/*****************************************************************************************************
**                                     Global Variables                                             **
*****************************************************************************************************/
uint_32 lastTransitionTime = 0;
uint_32 currentTime = millis();
uint_16 elapsedTime = 0;

volatile bool NorthSouthPedRequest = false;
volatile bool EastWestPedRequest = false;
volatile bool emergency = false;

// WebSocket message buffer
String messageBuffer;
WebSocketsClient webSocket;

/****************************************************************************************************
**                                     Functions                                                   **
****************************************************************************************************/


/**************************************************************************************************/
/* Function Name  : setup
   Function Type  : Initialization
   Parameters     : void
   Return         : void
   Discription    :This function initializes all the
                   one time executable commands
***************************************************************************************************/
void setup() {
  // Initialize Serial
  Serial.begin(115200);

  // Setup WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize WebSocket
  webSocket.begin(websocket_server, websocket_port);

  // Set event callbacks
  webSocket.onEvent(webSocketEvent);

  // Setup pin modes
  pinMode(NORTH_SOUTH_GREEN_PIN, OUTPUT);
  pinMode(NORTH_SOUTH_YELLOW_PIN, OUTPUT);
  pinMode(NORTH_SOUTH_RED_PIN, OUTPUT);
  pinMode(EAST_WEST_GREEN_PIN, OUTPUT);
  pinMode(EAST_WEST_YELLOW_PIN, OUTPUT);
  pinMode(EAST_WEST_RED_PIN, OUTPUT);
  pinMode(PED_NORTH_SOUTH_GREEN_PIN, OUTPUT);
  pinMode(PED_NORTH_SOUTH_RED_PIN, OUTPUT);
  pinMode(PED_EAST_WEST_GREEN_PIN, OUTPUT);
  pinMode(PED_EAST_WEST_RED_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  digitalWrite(NORTH_SOUTH_GREEN_PIN, LOW);
  digitalWrite(NORTH_SOUTH_YELLOW_PIN, LOW);
  digitalWrite(NORTH_SOUTH_RED_PIN, LOW);
  digitalWrite(EAST_WEST_GREEN_PIN, LOW);
  digitalWrite(EAST_WEST_YELLOW_PIN, LOW);
  digitalWrite(EAST_WEST_RED_PIN, LOW);
  digitalWrite(PED_NORTH_SOUTH_GREEN_PIN, LOW);
  digitalWrite(PED_EAST_WEST_GREEN_PIN, LOW);
  digitalWrite(PED_NORTH_SOUTH_RED_PIN, LOW);
  digitalWrite(PED_EAST_WEST_RED_PIN, LOW);

  pinMode(BUTTON_NORTH_SOUTH, INPUT_PULLUP);
  pinMode(BUTTON_EAST_WEST, INPUT_PULLUP);
  pinMode(EMERGENCY_BUTTON, INPUT_PULLUP);

  // Attach Interrupts with falling edges
  attachInterrupt(digitalPinToInterrupt(BUTTON_NORTH_SOUTH), NorthSouthPedButtonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_EAST_WEST), EastWestPedButtonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(EMERGENCY_BUTTON), emergencyISR, FALLING);
}

/**************************************************************************************************/
/* Function Name  : loop
   Function Type  : main function
   Parameters     : void
   Return         : void
   Discription    :This function executing the main program
                   algorithm to control the traffic lights
***************************************************************************************************/
void loop() {
  // Handle WebSocket connections
  webSocket.loop();

  currentTime = millis();

  switch (currentState) {
    case IDLE:                                                    // Intial Mode
      currentState = NORTH_SOUTH_GREEN;
      lastTransitionTime = currentTime;
      break;
    case NORTH_SOUTH_GREEN : NorthSouthGreen();   break;         // The north south traffic light is green Mode
    case NORTH_SOUTH_YELLOW: NorthSouthYellow();  break;         //The north south traffic light is yellow Mode
    case EAST_WEST_GREEN   : EastWestGreen();     break;         //The east west traffic light is green Mode
    case EAST_WEST_YELLOW  : EastWestYellow();    break;         //The east west traffic light is yellow Mode
    case PED_CROSS         : PedCross();          break;         //Pedstarin cross Mode
    case EMERGENCY         : Emergency();         break;         //Emergency Mode
	default            : currentState = IDLE; break;         //In case of error, return to Idle state
  }
}


/***************************************************************************************************/
/* Function Name  : DisplayNumbers
   Function Type  : Displaying
   Parameters     : integers for the numbers will be shown on seven sigments
   Return         : void
   Discription    :This function takes 2 numbers and display
                   them on the corresponding  seven segments
                   using Serial Input Parallel Output Shift registers
***************************************************************************************************/

void DisplayNumbers(uint_8 NorthSouth, uint_8 EastWest) {
  uint_8 segmentsNorthSouth = digitMap[NorthSouth]; // Get segment pattern for the number
  uint_8 segmentsEastWest   = digitMap[EastWest  ]; // Get segment pattern for the number
  digitalWrite(LATCH_PIN, LOW);  // Prepare for data transfer
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, segmentsNorthSouth); // Send data bit by bit
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, segmentsEastWest); // Send data bit by bit
  digitalWrite(LATCH_PIN, HIGH); // Latch the data to the output pins
}

/***************************************************************************************************/
/* Function Name  : NorthSouthGreen
   Function Type  : Executing Function
   Parameters     : void
   Return         : void
   Discription    :This function controls the traffic lights 
                   in the North South Green State
***************************************************************************************************/
void NorthSouthGreen()
{
  digitalWrite(NORTH_SOUTH_GREEN_PIN, HIGH);
  digitalWrite(NORTH_SOUTH_YELLOW_PIN, LOW);
  digitalWrite(NORTH_SOUTH_RED_PIN, LOW);
  digitalWrite(EAST_WEST_GREEN_PIN, LOW);
  digitalWrite(EAST_WEST_YELLOW_PIN, LOW);
  digitalWrite(EAST_WEST_RED_PIN, HIGH);
  digitalWrite(PED_NORTH_SOUTH_RED_PIN, HIGH);
  digitalWrite(PED_EAST_WEST_RED_PIN, LOW);
  digitalWrite(PED_NORTH_SOUTH_GREEN_PIN, LOW);
  digitalWrite(PED_EAST_WEST_GREEN_PIN, HIGH);
  elapsedTime = (currentTime - lastTransitionTime) / (1000);
  DisplayNumbers(GREEN_DURATION - elapsedTime, RED_DURATION - elapsedTime);       //Display the remaining durations

  if (currentTime - lastTransitionTime > GREEN_DURATION || NorthSouthPedRequest)  //In case of time out or there is pedstrain request
  {
    currentState = NORTH_SOUTH_YELLOW;
    lastTransitionTime = currentTime;
    NorthSouthPedRequest = false;
  }
  else
  {
    /*Do nothing */
  }
}

/***************************************************************************************************/
/* Function Name  : NorthSouthYellow
   Function Type  : Executing Function
   Parameters     : void
   Return         : void
   Discription    :This function controls the traffic lights 
                   in the North South Yellow State
***************************************************************************************************/
void NorthSouthYellow()
{
  digitalWrite(NORTH_SOUTH_GREEN_PIN, LOW);
  digitalWrite(NORTH_SOUTH_YELLOW_PIN, HIGH);
  digitalWrite(NORTH_SOUTH_RED_PIN, LOW);
  digitalWrite(EAST_WEST_GREEN_PIN, LOW);
  digitalWrite(EAST_WEST_YELLOW_PIN, LOW);
  digitalWrite(EAST_WEST_RED_PIN, HIGH);
  digitalWrite(PED_NORTH_SOUTH_RED_PIN, HIGH);
  digitalWrite(PED_EAST_WEST_RED_PIN, HIGH);
  digitalWrite(PED_NORTH_SOUTH_GREEN_PIN, LOW);
  digitalWrite(PED_EAST_WEST_GREEN_PIN, LOW);
  elapsedTime = (currentTime - lastTransitionTime) / (1000);
  DisplayNumbers(YELLOW_DURATION - elapsedTime, RED_DURATION - GREEN_DURATION - elapsedTime);     //Display the remaining durations
  //Red duration = Green duration + Yellow duration

  if (currentTime - lastTransitionTime > YELLOW_DURATION) {               //In case of time out
    currentState = EAST_WEST_GREEN;
    lastTransitionTime = currentTime;
  }
  else
  {
    /*Do nothing */
  }
}

/***************************************************************************************************/
/* Function Name  : EastWestGreen
   Function Type  : Executing Function
   Parameters     : void
   Return         : void
   Discription    :This function controls the traffic lights 
                   in the East West Green State
***************************************************************************************************/
void EastWestGreen()
{
  digitalWrite(NORTH_SOUTH_GREEN_PIN, LOW);
  digitalWrite(NORTH_SOUTH_YELLOW_PIN, LOW);
  digitalWrite(NORTH_SOUTH_RED_PIN, HIGH);
  digitalWrite(EAST_WEST_GREEN_PIN, HIGH);
  digitalWrite(EAST_WEST_YELLOW_PIN, LOW);
  digitalWrite(EAST_WEST_RED_PIN, LOW);
  digitalWrite(PED_NORTH_SOUTH_RED_PIN, LOW);
  digitalWrite(PED_EAST_WEST_RED_PIN, HIGH);
  digitalWrite(PED_NORTH_SOUTH_GREEN_PIN, HIGH);
  digitalWrite(PED_EAST_WEST_GREEN_PIN, LOW);
  elapsedTime = (currentTime - lastTransitionTime) / (1000);
  DisplayNumbers(RED_DURATION - elapsedTime, GREEN_DURATION - elapsedTime);                      //Display the remaining durations

  if (currentTime - lastTransitionTime > GREEN_DURATION || EastWestPedRequest)                   //In case of time out or there is pedstrain request
  {
    currentState = EAST_WEST_YELLOW;
    lastTransitionTime = currentTime;
    EastWestPedRequest = false;
  }
  else
  {
    /*Do nothing */
  }
}

/***************************************************************************************************/
/* Function Name  : EastWestYellow
   Function Type  : Executing Function
   Parameters     : void
   Return         : void
   Discription    :This function controls the traffic lights 
                   in the East West Yellow State
***************************************************************************************************/
void EastWestYellow()
{
  digitalWrite(NORTH_SOUTH_GREEN_PIN, LOW);
  digitalWrite(NORTH_SOUTH_YELLOW_PIN, LOW);
  digitalWrite(NORTH_SOUTH_RED_PIN, HIGH);
  digitalWrite(EAST_WEST_GREEN_PIN, LOW);
  digitalWrite(EAST_WEST_YELLOW_PIN, HIGH);
  digitalWrite(EAST_WEST_RED_PIN, LOW);
  digitalWrite(PED_NORTH_SOUTH_RED_PIN, HIGH);
  digitalWrite(PED_EAST_WEST_RED_PIN, HIGH);
  digitalWrite(PED_NORTH_SOUTH_GREEN_PIN, LOW);
  digitalWrite(PED_EAST_WEST_GREEN_PIN, LOW);
  elapsedTime = (currentTime - lastTransitionTime) / (1000);
  DisplayNumbers(RED_DURATION - GREEN_DURATION - elapsedTime, YELLOW_DURATION - elapsedTime);     //Display the remaining durations in the traffic lights
                                                                                                  //Red duration = Green duration + Yellow duration
  if (currentTime - lastTransitionTime > YELLOW_DURATION)                   //In case of time out
  {
    currentState = PED_CROSS;
    lastTransitionTime = currentTime;
  }
  else
  {
    /*Do nothing */
  }
}

/***************************************************************************************************/
/* Function Name  : PedCross
   Function Type  : Executing Function
   Parameters     : void
   Return         : void
   Discription    :This function controls the traffic lights 
                   in the Pedstrain Corss State
***************************************************************************************************/
void PedCross()
{
  digitalWrite(NORTH_SOUTH_GREEN_PIN, LOW);
  digitalWrite(NORTH_SOUTH_YELLOW_PIN, LOW);
  digitalWrite(NORTH_SOUTH_RED_PIN, HIGH);
  digitalWrite(EAST_WEST_GREEN_PIN, LOW);
  digitalWrite(EAST_WEST_YELLOW_PIN, LOW);
  digitalWrite(EAST_WEST_RED_PIN, HIGH);
  digitalWrite(PED_NORTH_SOUTH_RED_PIN, LOW);
  digitalWrite(PED_EAST_WEST_RED_PIN, LOW);
  digitalWrite(PED_NORTH_SOUTH_GREEN_PIN, HIGH);
  digitalWrite(PED_EAST_WEST_GREEN_PIN, HIGH);
  elapsedTime = (currentTime - lastTransitionTime) / (1000);
  DisplayNumbers(PED_DURATION - elapsedTime, PED_DURATION - elapsedTime);                    //Display the remaining Pedstarin duration in the traffic lights
  if (currentTime - lastTransitionTime > PED_DURATION) {                  //In case of time out
    currentState = NORTH_SOUTH_GREEN;
    lastTransitionTime = currentTime;
    digitalWrite(PED_NORTH_SOUTH_GREEN_PIN, LOW);
    digitalWrite(PED_EAST_WEST_GREEN_PIN, LOW);
    digitalWrite(PED_NORTH_SOUTH_RED_PIN, HIGH);
    digitalWrite(PED_EAST_WEST_RED_PIN, HIGH);
  }
  else
  {
    /*Do nothing */
  }
}

/***************************************************************************************************/
/* Function Name  : Emergency
   Function Type  : Executing Function
   Parameters     : void
   Return         : void
   Discription    :This function controls the traffic lights 
                   in the Emergency State
***************************************************************************************************/
void Emergency()
{
  digitalWrite(NORTH_SOUTH_GREEN_PIN, LOW);
  digitalWrite(NORTH_SOUTH_YELLOW_PIN, LOW);
  digitalWrite(NORTH_SOUTH_RED_PIN, HIGH);
  digitalWrite(EAST_WEST_GREEN_PIN, LOW);
  digitalWrite(EAST_WEST_YELLOW_PIN, LOW);
  digitalWrite(EAST_WEST_RED_PIN, HIGH);
  digitalWrite(PED_NORTH_SOUTH_GREEN_PIN, LOW);
  digitalWrite(PED_EAST_WEST_GREEN_PIN, LOW);
  digitalWrite(PED_NORTH_SOUTH_RED_PIN, HIGH);
  digitalWrite(PED_EAST_WEST_RED_PIN, HIGH);
  DisplayNumbers(0, 0);
  while (emergency);               // Wait for manual reset
  currentState = IDLE;
  lastTransitionTime = currentTime;
}

/**************************************************************************************************/
/* Function Name  : NorthSouthPedButtonISR
   Function Type  : Interrupt
   Parameters     : void
   Return         : void
   Discription    :This function activates when the north
                   south pedstrain button is pressed
***************************************************************************************************/
void NorthSouthPedButtonISR() {
  NorthSouthPedRequest = true;
}

/**************************************************************************************************/
/* Function Name  : EastWestPedButtonISR
   Function Type  : Interrupt
   Parameters     : void
   Return         : void
   Discription    :This function activates when the east
                   west pedstrain button is pressed
***************************************************************************************************/
void EastWestPedButtonISR() {
  EastWestPedRequest = true;
}

/**************************************************************************************************/
/* Function Name  : emergencyISR
   Function Type  : Interrupt
   Parameters     : void
   Return         : void
   Discription    :This function activates when the
                   emergency button is pressed
***************************************************************************************************/
void emergencyISR() {
  emergency = true;
}


/**************************************************************************************************/
/* Function Name  : webSocketEvent
   Function Type  : Interrupt
   Parameters     : type, payload, length
   Return         : void
   Discription    :This function handles the web socket
                   requests
***************************************************************************************************/
void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_TEXT) {
    String message = String((char*)payload);
    if (message == "NORTH_SOUTH_GREEN") {
      currentState = NORTH_SOUTH_GREEN;
    } else if (message == "NORTH_SOUTH_YELLOW") {
      currentState = NORTH_SOUTH_YELLOW;
    } else if (message == "EAST_WEST_GREEN") {
      currentState = EAST_WEST_GREEN;
    } else if (message == "EAST_WEST_YELLOW") {
      currentState = EAST_WEST_YELLOW;
    } else if (message == "PED_CROSS") {
      currentState = PED_CROSS;
    } else if (message == "EMERGENCY") {
      currentState = EMERGENCY;
    }else
    {
      /* Do nothing */
    }
  }
  else
  {
    /*Do nothing */
  }
}
