#include <wiringPi.h>
#include "includes.h"

using namespace std;

int main()
{
	wiringPiSetup(); 	// Setup the library
	 pinMode(0, OUTPUT); // Configure GPIO 0 as an output
	 pinMode(1, INPUT);  // Configure GPIO 1 as an input
		// digitalWrite(0,HIGH); delay(500);
		// digitalWrite(0,LOW);  delay(500);
		// if(digitalRead(1) == 1){} // example of read
	Core* core = new Core();
	core->Start();
}
