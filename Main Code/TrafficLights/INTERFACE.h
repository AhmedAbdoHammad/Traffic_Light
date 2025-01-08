/*******************************************************************************
**  FILENAME      : INTERFACE.h                             		          **
**                                                                            **
**  VERSION       : 1.0.0                                                     **
**                                                                            **
**  DATE          : 2025-01-06                                                **
**                                                                            **
**  PLATFORM      : ESP8266                                                   **
**                                                                            **
**  AUTHOR        : Ahmed Hammad                                              **
**                                                                            **
**  DESCRIPTION   : This file contains the declaration of the functions       **
** 																			  **
**  Can Be Changed: No (Static Code)                                          **
*******************************************************************************/

#ifndef INTERFACE.h
#define INTERFACE.h

/***************************************************************************************************/
/* Function Name  : DisplayNumbers
   Function Type  : Displaying
   Parameters     : int num for the number will be shown on
                    seven sigments
   Return         : void
   Discription    :This function takes 2 numbers and display
                   them on the corresponding  seven segments
                   using Serial Input Parallel Output Shift registers
***************************************************************************************************/

void DisplayNumbers(uint_8 NorthSouth, uint_8 EastWest);

/***************************************************************************************************/
/* Function Name  : NorthSouthGreen
   Function Type  : Executing Function
   Parameters     : void
   Return         : void
   Discription    :This function controls the traffic lights 
                   in the North South Green State
***************************************************************************************************/
void NorthSouthGreen();

/***************************************************************************************************/
/* Function Name  : NorthSouthYellow
   Function Type  : Executing Function
   Parameters     : void
   Return         : void
   Discription    :This function controls the traffic lights 
                   in the North South Yellow State
***************************************************************************************************/
void NorthSouthYellow();

/***************************************************************************************************/
/* Function Name  : EastWestGreen
   Function Type  : Executing Function
   Parameters     : void
   Return         : void
   Discription    :This function controls the traffic lights 
                   in the East West Green State
***************************************************************************************************/
void EastWestGreen();

/***************************************************************************************************/
/* Function Name  : EastWestYellow
   Function Type  : Executing Function
   Parameters     : void
   Return         : void
   Discription    :This function controls the traffic lights 
                   in the East West Yellow State
***************************************************************************************************/
void EastWestYellow();

/***************************************************************************************************/
/* Function Name  : PedCross
   Function Type  : Executing Function
   Parameters     : void
   Return         : void
   Discription    :This function controls the traffic lights 
                   in the Pedstrain Corss State
***************************************************************************************************/
void PedCross();

/***************************************************************************************************/
/* Function Name  : Emergency
   Function Type  : Executing Function
   Parameters     : void
   Return         : void
   Discription    :This function controls the traffic lights 
                   in the Emergency State
***************************************************************************************************/
void Emergency();

/**************************************************************************************************/
/* Function Name  : NorthSouthPedButtonISR
   Function Type  : Interrupt
   Parameters     : void
   Return         : void
   Discription    :This function activates when the north
                   south pedstrain button is pressed
***************************************************************************************************/
void NorthSouthPedButtonISR();

/**************************************************************************************************/
/* Function Name  : EastWestPedButtonISR
   Function Type  : Interrupt
   Parameters     : void
   Return         : void
   Discription    :This function activates when the east
                   west pedstrain button is pressed
***************************************************************************************************/
void EastWestPedButtonISR();

/**************************************************************************************************/
/* Function Name  : emergencyISR
   Function Type  : Interrupt
   Parameters     : void
   Return         : void
   Discription    :This function activates when the
                   emergency button is pressed
***************************************************************************************************/
void emergencyISR();

/**************************************************************************************************/
/* Function Name  : webSocketEvent
   Function Type  : Interrupt
   Parameters     : type, payload, length
   Return         : void
   Discription    :This function handles the web socket
                   requests
***************************************************************************************************/
void webSocketEvent(WStype_t type, uint8_t* payload, size_t length);

#endif
