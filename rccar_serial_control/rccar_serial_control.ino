/**
 *  Kuuve rc car serial control code
 *  @date 02.20.18
 */

#include <Servo.h>

Servo steer_motor;
Servo throt_motor;

/*Code for check data from remote controller
const int steer_in_pin = 5;
const int throt_in_pin = 6;
*/
const int steer_out_pin = 11;
const int throt_out_pin = 10;

int steer_in_value = 0;
int throt_in_value  = 0;
int steer_out_value = 1450;
int throt_out_value  = 1500;

void setup() {
  /*
  pinMode(steer_in_pin, INPUT);
  pinMode(throt_in_pin, INPUT);
  */

  steer_motor.attach(steer_out_pin);
  throt_motor.attach(throt_out_pin);

  Serial.begin(9200); // Starts the serial communication
}

void loop() {
  /*
  steer_in_value = pulseIn(steer_in_pin, HIGH);
  throt_in_value = pulseIn(throt_in_pin, HIGH);
  */

  if(Serial.available() > 0) {
    steer_in_value = Serial.parseInt();
    throt_in_value = Serial.parseInt();
    
    /* original data
     * throt: ~1450 (reverse), 1500~ (center: 1500)
     * steer: 1100~1900 (center:1450)
     * 
     * parsing data
     * throt: 1500, 1525~1540 -> 0~7
     * steer: 1100~1900 -> -7~7
     */
   
    if(throt_in_value == 0) {	// Stop
      throt_out_value = 1500;
	} else if(throt_in_value < 0) {	// Reverse
	  throt_out_value = 1450 + throt_in_value * (-2);
	} else {	// Forward
      throt_out_value = 1525 + (throt_in_value - 1) * 2;
    }
    steer_out_value = steer_in_value * 50 + 1450;
  }

  throt_motor.writeMicroseconds(throt_out_value);
  steer_motor.writeMicroseconds(steer_out_value);

  Serial.println(steer_out_value);
  Serial.println(throt_out_value);
}

