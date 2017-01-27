/**
 * @brief Arduino Driver v1.0 for The Cube project
 */
 
// Author: Gustavo Diaz

// Requiered Libraries
#include "cube1D.h"
#include "comInterface.h"

// Input pins for measured variables
#define HALL_PIN_IN 4
#define MPU_PIN 0x68    //I2C address for IMU

// Output pins for manipulated variables
#define ESC_PWM_PIN_OUT 9
#define ESC_DIR_PIN_OUT 6
#define SERVO_PIN_OUT 3

SoftwareSerial Bluetooth(7,8);    // RX, TX Arduino PINs, conected to Bluetooth

// Bluetooth Comunication Interface
char command = 's';
comInterface com_interface(&Bluetooth, &command, &Serial);

//Deiveces
Servo servo_freno;

// Device Control Handler
cube1D cube1d(HALL_PIN_IN, MPU_PIN, ESC_PWM_PIN_OUT, ESC_DIR_PIN_OUT, SERVO_PIN_OUT, &servo_freno, &Serial);

void setup()
{
	//Init Serial port
	Serial.begin(115200);
  Bluetooth.begin(115200);

  //Init Cube
  cube1d.init();
  Serial.println("Cube ready!");
  Serial.println("idle");
}

void loop()
{
  /* calibrate porpose rutine*/
  // cube1d.calibrateBreak();
  // delay(8000);
  // cube1d.jumpingUp(&servo_freno);
  // delay(8000);

  /* fixed behavior */
  // cube1d.balance(&servo_freno);
  // cube1d.printVariables();

  /* remote control (bluetooth) mode */
  com_interface.getCommand(); // Serial.println(command);
  cube1d.remoteControl(&command);
  cube1d.sendVariables(&Bluetooth);
}