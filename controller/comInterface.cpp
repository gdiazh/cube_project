/**
 * @brief Bluethooth HC-06 Manager Device Library
 */
 
// Author: Gustavo Diaz

// Requiered Libraries
#include "comInterface.h"

void comInterface::read(void)
{
	if(bt_port_->available())
	{
		bt_port_->flush();
		*command_ = bt_port_->read();
		debug_port_->print("dataReceived = ");
		// debug_port_->println(*command_);
		String cmd = (String) *command_;
		debug_port_->println(cmd);
		debug_print(command_);
	}
}

void comInterface::write(int data)
{
	bt_port_->write(data);
}

char comInterface::getCommand(void)
{
	read();
	return *command_;
}

void comInterface::sendCommand(int command)
{
	write(command);
}

void comInterface::debug_print(char *command)
{
    if (*command == 'j') debug_port_->println("[cube1D] INFO: Init jumpingUp");
    else if (*command == 'w') debug_port_->println("[cube1D] INFO: Running JumpingUp");
    else if (*command == 's') debug_port_->println("idle");
    else if (*command == 'b') debug_port_->println("balance");
}