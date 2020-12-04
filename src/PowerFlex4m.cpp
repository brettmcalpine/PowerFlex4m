#include "PowerFlex4m.h"

bool PowerFlex4m::init(uint8_t id) // Default pinouts
{
  SlaveID = id;
	// Start the Modbus RTU client
  if (!ModbusRTUClient.begin(9600)) {
    // Failed to start Modbus RTU Client!
		return false;
	}
  readLogicCommandRegister();
  frequencySource(powerflex_freq_comms);
  writeLogicCommandRegister();
	return true;
}

bool PowerFlex4m::direction()
{
	Forward_Reverse = readBitFromLogicStatus(POWERFLEX_FWD_REV);
	return Forward_Reverse;
}

void PowerFlex4m::direction(bool dir)
{
	Forward_Reverse = dir;
  writeBitToLogicCommand(POWERFLEX_FWD_REV_BIT1, Forward_Reverse);
	writeBitToLogicCommand(POWERFLEX_FWD_REV_BIT2, !Forward_Reverse);
  writeLogicCommandRegister();
}

void PowerFlex4m::frequency()
{
	Frequency = readRegister(POWERFLEX_REFERENCE);
}

void PowerFlex4m::frequency(float hz)
{
  Frequency = (uint16_t)(hz * 10);
	writeRegister(POWERFLEX_REFERENCE, Frequency);
}

bool PowerFlex4m::isRunning()
{
	Running = readBitFromLogicStatus(POWERFLEX_ACTIVE);
	return Running;
}

bool PowerFlex4m::isFaulted()
{
	Faulted = readBitFromLogicStatus(POWERFLEX_FAULTED);
	return Faulted;
}

void PowerFlex4m::start()
{
	writeBitToLogicCommand(POWERFLEX_START_BIT, 1);
	writeBitToLogicCommand(POWERFLEX_STOP_BIT, 0);
  writeLogicCommandRegister();
}

void PowerFlex4m::stop()
{
	writeBitToLogicCommand(POWERFLEX_START_BIT, 0);
	writeBitToLogicCommand(POWERFLEX_STOP_BIT, 1);
  writeLogicCommandRegister();
}

void PowerFlex4m::frequencySource(uint16_t source)
{
	writeBitToLogicCommand(POWERFLEX_FREQ_SOURCE_BIT3, bitRead(source, 0));
	writeBitToLogicCommand(POWERFLEX_FREQ_SOURCE_BIT2, bitRead(source, 1));
	writeBitToLogicCommand(POWERFLEX_FREQ_SOURCE_BIT1, bitRead(source, 2));
  writeBitToLogicCommand(POWERFLEX_REF_CTRLD_BY_COMM, true);
  writeLogicCommandRegister();
}

void PowerFlex4m::readFault()
{
	Fault = readRegister(POWERFLEX_DRIVE_ERROR_CODES);
}

void PowerFlex4m::printFault()
{
	for( unsigned int a = 0; a < sizeof(PFFD)/sizeof(PFFD[0]); a = a + 1 )
	{
		if (Fault == PFFD[a].code){
    	Serial.print(F("Fault: "));
			Serial.println(PFFD[a].description);
			break;
		}
	}
}

bool PowerFlex4m::isAccelerating()
{
	Accelerating = readBitFromLogicStatus(POWERFLEX_ACCELERATING);
  return Accelerating;
}

bool PowerFlex4m::isDecelerating()
{
	Decelerating = readBitFromLogicStatus(POWERFLEX_DECELERATING);
	return Decelerating;
}

void PowerFlex4m::setRelay(bool state)
{
  writeBitToLogicCommand(POWERFLEX_C_FORM_RELAY, state);
  writeLogicCommandRegister();
}

// R/W Functions

void PowerFlex4m::readLogicStatusRegister()
{
  LogicStatus = readRegister(POWERFLEX_LOGIC_STATUS_DATA);
}

void PowerFlex4m::readLogicCommandRegister()
{
  LogicCommand = readRegister(POWERFLEX_LOGIC_COMNMAND);
}

void PowerFlex4m::writeLogicCommandRegister()
{
  writeRegister(POWERFLEX_LOGIC_COMNMAND, LogicCommand);
}

int PowerFlex4m::readBitFromLogicCommand(int pos)
{
  return bitRead(LogicCommand, pos);
}

int PowerFlex4m::readBitFromLogicStatus(int pos)
{
  return bitRead(LogicStatus, pos);
}

void PowerFlex4m::writeBitToLogicCommand(int pos, bool value)
{
	bitWrite(LogicCommand, pos, value);
}

uint16_t PowerFlex4m::readRegister(uint16_t reg)
{
  uint16_t value = ModbusRTUClient.holdingRegisterRead(SlaveID, reg);
	return value;
}

void PowerFlex4m::writeRegister(uint16_t reg, uint16_t value)
{
	ModbusRTUClient.holdingRegisterWrite(SlaveID, reg, value);
}
