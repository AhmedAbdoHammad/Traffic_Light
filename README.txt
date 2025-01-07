********************************************************************* Auther       : Ahmed Hammad        *******************************************************************************
********************************************************************* Creation date: 5th of january 2025 *******************************************************************************

********************************************************************* Project States description ***************************************************************************************

* This project is mainly used to control 4 directions traffic lights with pedstrains.


*The traffic Lights mainly has 7 states which will be illustrated below: -
1) Idle state              : This is the very begining state for running the program. It redirects me to the NORTH_SOUTH_GREEN state.

2) NORTH_SOUTH_GREEN state : In this state, The green light in the north south, the red light in the east west traffic lights and the red lights for pedstrains 
			     will turned on and all other lights will be turned off. The counters will count for both lights. It redirects me to the NORTH_SOUTH_YELLOW state.

3) NORTH_SOUTH_YELLOW state: In this state, The yellow light in the north south, the red light in the east west traffic lights and the red lights for pedstrains
			     will turned on and all other lights will be turned off. The counters will count for both lights. It redirects me to the EAST_WEST_GREEN state.

4) EAST_WEST_GREEN state   : In this state, The green light in the east west, the red light in the north south traffic lights and the red lights for pedstrains will 
			     turned on and all other lights will be turned off. The counters will count for both lights. It redirects me to the EAST_WEST_YELLOW state.

5) EAST_WEST_YELLOW state  : In this state, The yellow light in the east west, the red light in the north south traffic lights and the red lights for pedstrains will 
			     turned on and all other lights will be turned off. The counters will count for both lights. It redirects me to the PEDSTRAIN_CROSS state.

6) PEDSTRAIN_CROSS state   : In this state, The green light for the pedstrains and the red lights for both the traffic lights will turned on  and all 
                             other lights will be turned off. The counters will count for both lights. It redirects me to the NORTH_SOUTH_GREEN state.

7) Emergency State         : In this state, all the red lights will be turned on and turn off all other lights. The counters will show zero and will wait for manual reset


*The traffic Lights can also be cotrolled using a web server which will help us in case of specific emergency like ambulance or police car needs to pass emergently

******************************************************************* Hardware Implementation ********************************************************************************************
* 1 ESP8266 Module (It is used to be connected to the web server)
* 2 Serial input parallel output shift register 74HC595 (They are used to increase the number of GPIO pins in the ESP8266 to control 2 7-segments
* 2 Red leds, 2 Yellow leds, 4 Green leds, 8 resistors 330 ohm
* 2 7-segments common cathode to count each light number
* 3 Buttons for pedstrains and emrgency

******************************************************************* Configurations ******************************************************************************************************
We have set of configurations can be set in the program including: -
1) Leds and buttons Pins
2) Wifi SSID and Password
3) Web Socket IP and Port
4) Red, Green, Yellow Durations

******************************************************************* Others ******************************************************************************************************
For any problems or asking for something, reach me out on: ahmedabdo.hammad@gmail.com

Thanks for your time








