/**
 * @brief Bluethooth HC-06 Manager Device Library
 */
 
// Author: Gustavo Diaz

// Requiered Libraries
#include <Arduino.h>
#include <SoftwareSerial.h>

class comInterface
{
    // Members
    SoftwareSerial *bt_port_;
    HardwareSerial *debug_port_;
    char *command_;

public:
    // Public Members
    // ...

    // constructor de base (null)
    comInterface() {}

    // constructror parametrizado
    comInterface(SoftwareSerial *bt_port, char *command, HardwareSerial *debug_port):
        bt_port_(bt_port),
        debug_port_(debug_port),
        command_(command)
    {}

    // methods
    char getCommand(void);
    void sendCommand(int command);
    void debug_print(char *command);

private:
    // methods
    
    void read(void);
    void write(int data);

};