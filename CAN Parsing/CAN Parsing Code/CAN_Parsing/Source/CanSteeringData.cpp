#include "../Headers/CanSteeringData.h"
#include "../Headers/CanData.h"

// constructor
CanSteeringData::CanSteeringData(int id, double ts, int dc, CanData::DataType dt, string* data)
{
	setIdentifier(id);
	setTimestamp(ts);
	setDataCount(dc);
	setDataString(data);
	convertStringMessageToU64(data);
	parseCanMessage();
}


// Whole bunch of getters
// No setters because we want those values to not changes after object is created
double CanSteeringData::getWheelAngle()
{
	return m_wheelAngle;
}

int CanSteeringData::getAxleNumber()
{
	return m_axleNumber;
}

int CanSteeringData::getWheelNumber()
{
	return m_wheelNumber;
}

int CanSteeringData::getAxleStatus()
{
	return m_axleStatus;
}

int CanSteeringData::getSteeringType()
{
	return m_steeringType;
}

int CanSteeringData::getSteeringForceType()
{
	return m_steeringForceType;
}

int CanSteeringData::getSteeringForceTransmission()
{
	return m_steeringForceTransmission;
}

// Calls various functions to parse data from message
bool CanSteeringData::parseCanMessage()
{
	determineWheelAngle();
	determineAxleLocation();
	determineWheelLocation();
	return true;
}

void CanSteeringData::determineWheelAngle()
{
	unsigned __int64 canData = 0U;
	getMessageData(&canData);
	unsigned __int16 wheelAngleBytes = canData >> 48;
	unsigned __int16 lowerWheelByte = wheelAngleBytes >> 8;
	unsigned __int16 upperWheelByte = (wheelAngleBytes & 0x00FF) << 8;
	m_wheelAngle = (((double)(upperWheelByte | lowerWheelByte)) / 256) - 125;
	//cout << "Found angle " << m_wheelAngle;
}

void CanSteeringData::determineAxleLocation()
{
	unsigned __int64 canData = 0U;
	getMessageData(&canData);
	m_axleNumber = (canData & 0x0000F00000000000) >> 44;
	//cout << " for axle number " << (int)m_axleNumber;
}

void CanSteeringData::determineWheelLocation()
{
	unsigned __int64 canData = 0U;
	getMessageData(&canData);
	m_wheelNumber = (canData & 0x00000F0000000000) >> 40;
	//cout << " for wheel number " << (int)m_wheelNumber << endl;
}

// need a single function that takes the data from base class and fills in teh above.
// actually, make it the constructor?