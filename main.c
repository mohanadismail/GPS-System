#include "Headers/tm4c123gh6pm.h"
#include "karim.h"
#include "7_segments.h"
#include "GPGGA_functions.h"
#include "Extract-Coordinates.h"
#include "distanceBetweenTwoPoints.h"

//Global variables
float distance = 0;
float prev_lat, prev_long, current_lat, current_long;
//char str[150]; (externed in GPGGA_functions.h) (to save GPGGA message in)
//char *data[15]; (externed in GPGGA_functions.h) (to split GPGGA into tokens)

int main () {
	//All initilizations
	initUART();
	segments_initialize();
	PORTF_init();
	led_on('r'); //turn red led on until gps fix
	extract_GPGGA_message(); //take UART input
	split_GPGGA();
	while (data_length < 6) {  // to avoid segmentation fault
		extract_GPGGA_message();
		split_GPGGA();
	}
	while (!validity(data)) { //wait till gps fix
	extract_GPGGA_message();
	split_GPGGA();
	}
	current_lat = getLatitude(data); //save current location
	current_long = getLongitude(data);
	led_on('g'); //turn green led on (gps fixed you may start moving now)
	while (1) {
		if (isHere(current_long, current_lat)) { //check if reached the destination
			led_on('b'); //turn blue led on (you reached the destination)
			print((int)distance); //show distance on 7-segments
			break;
		}
		extract_GPGGA_message(); //take new input
		split_GPGGA();
		prev_lat = current_lat;
		prev_long = current_long;
		current_lat = getLatitude(data);
		current_long = getLongitude(data);
		distance += distanceBetweenTwoPoints(prev_lat, prev_long, current_lat, current_long); //calculate distance and increase calculated distance
		print((int)distance); //show distance on seven segments while moving
	}
}
