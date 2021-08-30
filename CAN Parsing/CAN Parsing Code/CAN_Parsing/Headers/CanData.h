/* This file holds header data for the CanData class*/
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class CanData
{
	

public:

	enum class DataType
	{
		minVal = 0,
		unknownVal = 1,
		steeringVal = 2
	};

	static const int STEERING_MESSAGE_NUMBER = 418384768;

	CanData();
	virtual int getIdentifier();
	int getDataCount();
	double getTimestamp();
	string* getMessageData();
	void setTimestamp(const double);
	void setIdentifier(const unsigned int);
	void setDataCount(const unsigned int);
	void setDataString(string*);
	void convertStringMessageToU64(string*);
	void getMessageData(unsigned __int64*);
	DataType getDataType();

private:

	unsigned __int64 m_messageDataU64;
	unsigned int m_identifier;
	double m_timestamp;
	unsigned int m_dataCount;
	DataType m_dataType;
	string * m_data;
	void setDataType();
};