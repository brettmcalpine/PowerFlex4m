/*
	IMC.h - Library for running standard industrial motors over standard inducstrial communications
	Created by Brett S. McAlpine
	Released into public domain
*/

#ifndef _IMC_h
#define _IMC_h

// Allen Bradley uses RS-485 (i.e. Serial)
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>

// For Lenze
//#include <CAN.h>

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// Generic defines
#define MOTOR_FORWARD												1
#define MOTOR_REVERSE												0

// PowerFlex 4m Addresses
#define	POWERFLEX_LOGIC_COMNMAND						8192 // Writing "06" 0x2000
#define	POWERFLEX_REFERENCE									8193 // Writing "06" 0x2001 decimal x 10
#define	POWERFLEX_LOGIC_STATUS_DATA					8448 // Reading "03" bits
#define POWERFLEX_DRIVE_ERROR_CODES					8449 // Reading "03" decimal

// PowerFlex 4m Write bitmask positions for POWERFLEX_LOGIC_COMNMAND
#define	POWERFLEX_STOP_BIT									0x00
#define	POWERFLEX_START_BIT									0x01
#define	POWERFLEX_JOG_BIT										0x02
#define	POWERFLEX_CLEAR_FAULTS_BIT					0x03
#define	POWERFLEX_FWD_REV_BIT1							0x04
#define	POWERFLEX_FWD_REV_BIT2							0x05
#define	POWERFLEX_C_FORM_RELAY							0x06
#define	POWERFLEX_MOP_INCREMENT							0x07
#define	POWERFLEX_ACCEL_RATE_BIT2						0x08
#define	POWERFLEX_ACCEL_RATE_BIT1						0x09
#define	POWERFLEX_DECEL_RATE_BIT2						0x0A
#define	POWERFLEX_DECEL_RATE_BIT1						0x0B
#define	POWERFLEX_FREQ_SOURCE_BIT3					0x0C
#define	POWERFLEX_FREQ_SOURCE_BIT2					0x0D
#define	POWERFLEX_FREQ_SOURCE_BIT1					0x0E
#define	POWERFLEX_MOP_DECREMENT							0x0F

// PowerFlex 4m Read bitmask position for POWERFLEX_LOGIC_STATUS_DATA
#define	POWERFLEX_READY											0x00
#define	POWERFLEX_ACTIVE										0x01
#define	POWERFLEX_FWD_REV										0x02
#define	POWERFLEX_ROTATING_FWD_REV					0x03
#define	POWERFLEX_ACCELERATING							0x04
#define	POWERFLEX_DECELERATING							0x05
#define	POWERFLEX_ALARM											0x06
#define	POWERFLEX_FAULTED										0x07
#define	POWERFLEX_AT_REFERENCE							0x08
#define	POWERFLEX_REF_CTRLD_BY_COMM					0x09
#define	POWERFLEX_PARAMS_LOCKED							0x0A
#define	POWERFLEX_DIGITAL_IN_1							0x0B
#define	POWERFLEX_DIGITAL_IN_2							0x0C

// PowerFlex 4m error codes for POWERFLEX_DRIVE_ERROR_CODES
typedef struct {
  uint8_t code;
  char* description;
} PowerFlexFaultDictionary;
const PowerFlexFaultDictionary PFFD[] PROGMEM{
	{0, "powerflex_no_fault"},
	{2, "powerflex_aux_input"},
	{3, "powerflex_power_loss"},
	{4, "powerflex_undervoltage"},
	{5, "powerflex_overvoltage"},
	{6, "powerflex_motor_stalled"},
	{7, "powerflex_motor_overload"},
	{8, "powerflex_heatsink_overtemp"},
	{12, "powerflex_hw_overcurrent_300pc"},
	{13, "powerflex_ground_fault"},
	{29, "powerflex_analog_input_loss"},
	{33, "powerflex_auto_restart_tries"},
	{38, "powerflex_phase_u_ground_short"},
	{39, "powerflex_phase_v_ground_short"},
	{40, "powerflex_phase_w_ground_short"},
	{41, "powerflex_phase_u_v_short"},
	{42, "powerflex_phase_u_w_short"},
	{43, "powerflex_phase_v_w_short"},
	{63, "powerflex_software_overcurrent"},
	{64, "powerflex_drive_overload"},
	{70, "powerflex_power_unit_fail"},
	{80, "powerflex_autotune_fail"},
	{81, "powerflex_communication_loss"},
	{100, "powerflex_parameter_checksum_error"},
	{122, "powerflex_io_board_fail"},
};

// PowerFlex frequency source options
#define powerflex_freq_P108_ref 				 1
#define powerflex_freq_A409_int 				 2
#define powerflex_freq_comms					 	 3
#define powerflex_freq_preset0					 4
#define powerflex_freq_preset1					 5
#define powerflex_freq_preset2					 6
#define powerflex_freq_preset3					 7

// Base Class with standard motor functions
class IMC
{
public:
	bool Forward_Reverse;
	bool Running;
	bool Faulted;
	bool Accelerating;
	bool Decelerating;
};

// Allen Bradley Specific
class PowerFlex4m : private IMC
{
public:
	uint16_t Frequency;
	uint16_t Fault;
	uint8_t SlaveID;
	uint16_t LogicCommand;
	uint16_t LogicStatus;

	bool init(uint8_t id);
	bool direction();
	void direction(bool dir);
	void frequency();
	void frequency(float hz);
	bool isRunning();
	bool isFaulted();
	void start();
	void stop();

	void frequencySource(uint16_t source);					// Pick from list
	void printFault();

	bool isAccelerating();
	bool isDecelerating();

	void setRelay(bool state);

private:
	// Specific to Allen Bradley PowerFlex 4m
	void readLogicStatusRegister();
	void readFault();

	int readBitFromLogicCommand(int pos);
	void writeBitToLogicCommand(int pos, bool value);

	int readBitFromLogicStatus(int pos);

	void readLogicCommandRegister();
	void writeLogicCommandRegister();

	uint16_t readRegister(uint16_t reg);
	void writeRegister(uint16_t reg, uint16_t value);
};

#endif
