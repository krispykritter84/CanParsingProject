#pragma once
#include "CanData.h"

using namespace std;

class CanSteeringData : public CanData {
public:
	CanSteeringData(int id, double ts, int dc, CanData::DataType dt, string* data);
	// theses are all available in the message but we don't need all of them in this example
	//int getIdentifier();
	double getWheelAngle();
	int getAxleNumber();
	int getWheelNumber();
	int getAxleStatus();
	int getSteeringType();
	int getSteeringForceType();
	int getSteeringForceTransmission();

private:
	double m_wheelAngle;
	unsigned int m_axleNumber;
	unsigned int m_wheelNumber;
	int m_axleStatus;
	int m_liftAxleLoweringInhibit;
	int m_steeringType;
	int m_steeringForceType;
	int m_steeringForceTransmission;

	bool parseCanMessage();
	void determineWheelAngle();
	void determineAxleLocation();
	void determineWheelLocation();
};