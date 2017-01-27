/**
 * @brief Cube Manager Device Library
 */
 
// Author: Gustavo Diaz

#include "cube1D.h"

//-------------------------- Define Math functions -----------------------------------------
float cube1D::torque2pwm(float value, float toLow, float toHigh)
{
  float fromLow = 0;
  float fromHigh = MAX_TORQUE;
  float new_range = toHigh-toLow;
  float old_range = fromHigh - fromLow;
  if (value < -fromHigh || value > fromHigh)
  {
    debug_port_->println('Warning value out of range');
    value = fromHigh;
  }
  if (value<0) value = -value;
  float new_value = (new_range/old_range)*(value-fromLow)+toLow;
  return new_value;
}

//-------------------------- Define functions to get measurements --------------------------
void cube1D::initMPU(void)
{
    Wire.begin();
    Wire.beginTransmission(mpu_pin_);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
}
void cube1D::updateOrientation(unsigned long dt)
{
    float dt_sec = (float) dt;
    //Leer los valores del Acelerometro de la IMU
   Wire.beginTransmission(mpu_pin_);
   Wire.write(0x3B); //Pedir el registro 0x3B - corresponde al AcX
   Wire.endTransmission(false);
   Wire.requestFrom(mpu_pin_,6,true); //A partir del 0x3B, se piden 6 registros
   mpu_vars_.AcX=Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 registros
   mpu_vars_.AcY=Wire.read()<<8|Wire.read();
   mpu_vars_.AcZ=Wire.read()<<8|Wire.read();
 
    //A partir de los valores del acelerometro, se calculan los angulos Y, X
    //respectivamente, con la formula de la tangente.
   mpu_vars_.Acc[1] = atan(-1*(mpu_vars_.AcX/A_R)/sqrt(pow((mpu_vars_.AcY/A_R),2) + pow((mpu_vars_.AcZ/A_R),2)))*RAD_A_DEG;
   mpu_vars_.Acc[0] = atan((mpu_vars_.AcY/A_R)/sqrt(pow((mpu_vars_.AcX/A_R),2) + pow((mpu_vars_.AcZ/A_R),2)))*RAD_A_DEG;
 
   //Leer los valores del Giroscopio
   Wire.beginTransmission(mpu_pin_);
   Wire.write(0x43);
   Wire.endTransmission(false);
   Wire.requestFrom(mpu_pin_,4,true); //A diferencia del Acelerometro, solo se piden 4 registros
   mpu_vars_.GyX=Wire.read()<<8|Wire.read();
   mpu_vars_.GyY=Wire.read()<<8|Wire.read();
 
   //Calculo del angulo del Giroscopio
   mpu_vars_.Gy[0] = mpu_vars_.GyX/G_R;
   mpu_vars_.Gy[1] = mpu_vars_.GyY/G_R;
 
   //Aplicar el Filtro Complementario
   mpu_vars_.Angle[0] = 0.98 *(mpu_vars_.Angle[0]+mpu_vars_.Gy[0]*0.001) + 0.02*mpu_vars_.Acc[0];
   mpu_vars_.Angle[1] = 0.98 *(mpu_vars_.Angle[1]+mpu_vars_.Gy[1]*0.001) + 0.02*mpu_vars_.Acc[1];
   // mpu_vars_.Angle[0] = 0.98 *(mpu_vars_.Angle[0]+mpu_vars_.Gy[0]*dt_sec) + 0.02*mpu_vars_.Acc[0];
   // mpu_vars_.Angle[1] = 0.98 *(mpu_vars_.Angle[1]+mpu_vars_.Gy[1]*dt_sec) + 0.02*mpu_vars_.Acc[1];
 
   // delay(1); //Nuestra dt sera, pues, 0.010, que es el intervalo de tiempo en cada medida
}

//-------------------------- Define Handlers functions -----------------------------------------------
float cube1D::motor_dynamic(float torque)
{
    int vel = torque; //TO DO: REPLACE FOR PARTICULAR DYNAMICS: w = Te/(Js+B)
    return vel;
}

void cube1D::init_esc()
{
    esc_.writeMicroseconds(1000);   //1000[us] = 1[ms]
    delay(5000);           //necesary to init the ESC
}

int cube1D::pwm_modulation(float vel_control)
{
    int pwm_duty_cycle = (int) torque2pwm(vel_control, 1405, 1415);
    return pwm_duty_cycle;
}

int cube1D::direction_handler(float vel_control)
{
    if (vel_control>=0) return 1;
    else return 0;
}

//-------------------------- Public High Level Abstract Methods --------------------------
void cube1D::init(void)
{
    // Assign ouput pins
    pinMode(esc_pwm_pin_, OUTPUT);
    pinMode(esc_dir_pin_, OUTPUT);
    pinMode(servo_pin_, OUTPUT);

    esc_.attach(esc_pwm_pin_);
    servo_->attach(servo_pin_);

    initMPU();

    init_esc();

    servo_->write(RELEASE);

    // initialize state variables
    state_vars_.cube_angle = 0.1;   //[rad]
    state_vars_.cube_vel = 0;       //[rad/s]
    state_vars_.motor_vel = 0;      //[rad/s]
    // initialize control variables
    control_vars_.error = 0;            //[rad/s]
    control_vars_.control_input = 0;    //[A]
    control_vars_.torque_input = 0;     //[Nm]
    control_vars_.dir_has_change = 0;   //[1:Y, 0:N]
    control_vars_.init_count = 0;       //[1:Y, 0:N]
    control_vars_.break_delay = 0;      //[int]
    control_vars_.dt = 0.001;           //[s]
    // initialize output variables
    output_vars_.output_vel = 0;     //[RPM]
    output_vars_.output_pwm = 1400;  //[us]
    output_vars_.output_dir = 0;     //[0:right, 1:left]

    last_cmd_ = 's';  //idle state
    jump_state_ = 0;
}

void cube1D::idle(Servo *servo)
{
    updateVariables();
    servo->write(RELEASE);
    delay(5);
    esc_.writeMicroseconds(1000);
}

void cube1D::calibrateBreak(void)
{
  servo_->write(RELEASE);
  delay(3000);
  servo_->write(BREAK);
  delay(3000);
}

void cube1D::jumpingUp(Servo *servo)
{
    jump_state_ = 1;
    // delay(JUMPING_DELAY);
    esc_.writeMicroseconds(JUMPING_VEL);
    delay(JUMPING_DELAY);
    esc_.writeMicroseconds(1000);

    servo->write(BREAK);
    delay(JUMP_BREAK_DELAY);
    servo->write(RELEASE);
    jump_state_ = 0;
}

void cube1D::updateVariables(void)
{
    // Update mesures
    updateOrientation(control_vars_.dt*0.001);
    control_vars_.dt = millis();
    state_vars_.cube_angle = mpu_vars_.Angle[0]*DEG_A_RAD;
    state_vars_.cube_vel = mpu_vars_.Gy[0]*DEG_A_RAD;

    // Compute error and control variables
    control_vars_.error = SET_POINT - (K1*state_vars_.cube_angle+K2*state_vars_.cube_vel);
    control_vars_.control_input = control_vars_.error;
    control_vars_.torque_input = control_vars_.control_input*Km;
    // control_vars_.torque_input >=1000?1000:control_vars_.torque_input;
    // control_vars_.torque_input <=-1000?-1000:control_vars_.torque_input;
    if (control_vars_.torque_input>MAX_TORQUE || control_vars_.torque_input<-MAX_TORQUE)
    {
        if (control_vars_.torque_input>MAX_TORQUE) control_vars_.torque_input = MAX_TORQUE;
        else control_vars_.torque_input = -MAX_TORQUE;
    }
    output_vars_.output_vel = motor_dynamic(control_vars_.torque_input);
    output_vars_.output_pwm = pwm_modulation(output_vars_.output_vel);
    output_vars_.output_dir = direction_handler(output_vars_.output_vel);
}

void cube1D::balance(Servo *servo)
{
    updateVariables();
    // Update outputs
    if (output_vars_.output_dir == 1) digitalWrite(esc_dir_pin_, LOW);
    else digitalWrite(esc_dir_pin_, HIGH);
    esc_.writeMicroseconds(output_vars_.output_pwm);
    // delay(200);
    if(control_vars_.dir_has_change != output_vars_.output_dir)
    {
    control_vars_.init_count = 1;
    }
    if (control_vars_.init_count) control_vars_.break_delay++;
    if (control_vars_.break_delay>=255)
    {
        // Serial.println("activate servo");
        servo->write(BREAK);
        delay(250);
        servo->write(RELEASE);
        control_vars_.dir_has_change = output_vars_.output_dir;
        control_vars_.init_count = 0;
        control_vars_.break_delay = 0;
    }
    control_vars_.dt = millis() - control_vars_.dt;
}

void cube1D::printVariables(void)
{
    debug_port_->print(state_vars_.cube_angle*RAD_A_DEG);
    debug_port_->print(";");
    debug_port_->print(state_vars_.cube_vel*RAD_A_DEG);
    debug_port_->print(";");
    debug_port_->print(control_vars_.torque_input);
    debug_port_->print(";");
    debug_port_->print(output_vars_.output_pwm);
    debug_port_->print(";");
    debug_port_->print(output_vars_.output_dir);
    debug_port_->println(";");
}

void cube1D::remoteControl(char *command)
{
    if (*command == 'j' && jump_state_ == 0)
    {
        // debug_port_->println("[cube1D] INFO: Init jumpingUp");
        *command = 'w';  // Wait for JumpingUp to end
        last_cmd_ = 'w';
        jumpingUp(servo_);
    }
    else if (*command == 'w' && jump_state_ == 1) // Running JumpingUp
    {
        last_cmd_ = 'w';
        // debug_port_->println("[cube1D] INFO: Running JumpingUp");
    }
    else if (*command == 'w' && jump_state_ == 0)  // JumpingUp finished
    {
        debug_port_->println("[cube1D] INFO: End jumpingUp");
        *command = 's';
        last_cmd_ = 's';
        idle(servo_);
    }
    else if (*command == 's')
    {
        idle(servo_);
        last_cmd_ = 's';
        // debug_port_->println("idle");
    }
    else if (*command == 'b')
    {
        balance(servo_);
        last_cmd_ = 'b';
        // debug_port_->println("balance");
    }
    else
    {
      // debug_port_->println("[cube1D] WARN: Unknown command");
      *command = last_cmd_;
    }
}

void cube1D::sendVariables(SoftwareSerial *bt_port)
{
    bt_port->flush();
    bt_port->print(state_vars_.cube_angle*RAD_A_DEG);
    bt_port->print(";");
    bt_port->print(state_vars_.cube_vel*RAD_A_DEG);
    bt_port->print(";");
    bt_port->print(control_vars_.torque_input);
    bt_port->print(";");
    bt_port->print(output_vars_.output_pwm);
    bt_port->print(";");
    bt_port->print(output_vars_.output_dir);
    bt_port->println(";");
    // bt_port->flush();
}

void cube1D::moveServo(Servo *servo)
{
    servo->write(RELEASE);
}