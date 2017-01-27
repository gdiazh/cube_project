/**
 * @brief Cube Manager Device Library
 */

// Author: Gustavo Diaz

// Requiered Libraries
#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// System behavior
#define SET_POINT 0
#define JUMPING_VEL 1550
#define JUMPING_DELAY 4000

// Controller parameters
#define K1 -11561.15
#define K2 -1236.20
#define K3 -17.32

// Motor parameters
#define Km 0.0251
#define K_ESC 100
#define MAX_TORQUE 300

// Break parameters
#define BREAK 110
#define RELEASE 80
#define JUMP_BREAK_DELAY 2000

// Convertion constants
#define A_R 16384.0 //convertion ratios for MPU
#define G_R 131.0
#define RAD_A_DEG 57.2957
#define DEG_A_RAD 0.0174

typedef struct StateVariables{
    float cube_angle;
    float cube_vel;
    float motor_vel;
} StateVariables;

typedef struct ControlVariables{
    float error;
    float control_input;
    float torque_input;
    int dir_has_change;
    int init_count;
    int break_delay;
    unsigned long dt;
} ControlVariables;

typedef struct OutputVariables{
    float output_vel;
    float output_pwm;
    int output_dir;
} OutputVariables;

typedef struct MPUVariables{
    int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
    float Acc[2];
    float Gy[2];
    float Angle[2];
} MPUVariables;

/**
 * @class cube1D
 * @brief Class for manage The Cube.
 */

class cube1D
{
    // Members
    uint8_t hall_pin_;
    uint8_t mpu_pin_;
    uint8_t esc_pwm_pin_;
    uint8_t esc_dir_pin_;
    uint8_t servo_pin_;

    Servo *servo_;
    Servo esc_;

    StateVariables state_vars_;
    ControlVariables control_vars_;
    OutputVariables output_vars_;
    MPUVariables mpu_vars_;

    HardwareSerial *debug_port_;

    char last_cmd_;
    int jump_state_;

public:
    // Public Members
    // ControlVariables control_vars_;

    // constructor de base (null)
    cube1D() {}

    // constructror parametrizado
    cube1D(uint8_t hall_pin, uint8_t mpu_pin, uint8_t esc_pwm_pin, uint8_t esc_dir_pin, uint8_t servo_pin, Servo *servo, HardwareSerial *debug_port):
        hall_pin_(hall_pin),
        mpu_pin_(mpu_pin),
        esc_pwm_pin_(esc_pwm_pin),
        esc_dir_pin_(esc_dir_pin),
        servo_pin_(servo_pin),
        servo_(servo),
        /*esc_(),
        state_vars_(),
        control_vars_(),
        output_vars_(),
        mpu_vars_(),*/
        debug_port_(debug_port)
    {
        // Config pins
        /*pinMode(servo_pin_, OUTPUT);
        pinMode(esc_pwm_pin_, OUTPUT);*/
    }

    // methods
    void init(void);
    void idle(Servo *servo);
    void jumpingUp(Servo *servo);
    void calibrateBreak(void);
    void balance(Servo *servo);
    void printVariables(void);
    void remoteControl(char *command);
    void sendVariables(SoftwareSerial *bt_port);
    void moveServo(Servo *servo);

private:
    // methods
    
    // Declaration for Math functions
    float torque2pwm(float value, float toLow, float toHigh);

    // Declaration for functions to get measurements
    void initMPU(void);
    void updateOrientation(unsigned long dt);
    void updateVariables(void);


    // Declaration of Output Handlers
    float motor_dynamic(float torque);
    void init_esc();
    int pwm_modulation(float vel_control);
    int direction_handler(float vel_control);

};