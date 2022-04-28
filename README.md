## **Purpose**

The purpose of this project is to be able to balance a ball in the middle of a beam.
To do this I used a servo motor (Futaba S3003) to move the beam, an ultrasonic sensor (HCSR04) to measure
the distance of the ball, the A-Start 32u4 Robot controlle LV, and 4 potentiometers for future tuning. Ultimately the 
whole project is controlled by a PID controller inside the main while loop. There 
the pid controller calculates the error from the set point we used, ex. 14 cm is the middle and 
the distance measured from the ultrasonic sensor. It then uses this error to move the servo angle.
This project uses all proportional, derivative and integral control. Integral control can be left out,
but might not get to exactly the middle, but seems to still have good results.


## **Contents**

1. HCSRO4.h and c file
2. servo.h and c file
3. gpio.h and c file
4. leds.h and c file
5. Analog.h and c file
6. serial.h and c file
7. common.h
8. main.c
9. Makefile
10. .gitignore
11. README.md

## **Notes**

Using serial communication in the while loop will affect the PID controller and therefore not give you good results.
Use serial communication when you feel the ultrasonic sensor is not reading values correctly. When I placed mine, sometimes I would get readings that were off
by a lot, and had to glue my sensor in a certain angle for it to measure correctly. Sometimes I wasn't sure if the pots were reading in the direction I wanted
so you could also check that with some serial communication. The leds can also be excluded from this project and replaced with tape flags or similar when trying to 
tune the pid values as it adds overhead when polling for it inside the while loop. I also chose the pins carefully to be able to open up serial communication easily
by removing the leds plugged into pin 0 and pin 1 (green and white) you open up the pins to be used for serial communication, and the red led can be left as is.
