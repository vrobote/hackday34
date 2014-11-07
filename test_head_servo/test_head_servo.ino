//
// test head servo
//
// http://robocraft.ru
//

#include <Servo.h>

#define SERVO_COUNT 3

Servo servo[SERVO_COUNT];

int head_yaw = 90;
int head_roll = 90;
int head_pitch = 90;

void setup()
{
    Serial.begin(57600);

    servo[0].attach(3);
    servo[1].attach(4);
    servo[2].attach(5);

    Serial.println(F("[i] Start..."));
}

void loop()
{
    int c;
    if(Serial.available()) {
        c = Serial.read();
        if(c == 'w') {
            head_pitch++;
        }
        else if(c == 's') {
            head_pitch--;
        }
        else if(c == 'a') {
            head_yaw++;
        }
        else if(c == 'd') {
            head_yaw--;
        }
        else if(c == 'q') {
            head_roll++;
        }
        else if(c == 'e') {
            head_roll--;
        }
        else {

        }
    }
    servo[0].write(head_yaw);
    delay(100);
    servo[1].write(head_roll);
    delay(100);
    servo[2].write(head_pitch);
    delay(100);

    Serial.print("Y: ");
    Serial.print(head_yaw);
    Serial.print(" R: ");
    Serial.print(head_roll);
    Serial.print(" P: ");
    Serial.print(head_pitch);
    Serial.println("");
    delay(100);
}
