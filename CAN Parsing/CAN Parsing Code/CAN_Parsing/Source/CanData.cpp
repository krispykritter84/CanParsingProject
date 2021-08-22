/* Base class to hold all the data that is contained 
   in every CAN message we'll receive.
   
   Having to take it on faith that the file won't be 
   too screwed up when we test this.  I'd add more
   error checking and handling if more time was
   available  
*/
#include "../Headers/CanData.h"

// constructor
CanData::CanData()
{
	m_identifier = 0;
	m_timestamp = 0;
	m_dataCount = 0;
	m_dataType = DataType::minVal;
	m_messageDataU64 = 0U;
	m_data = new string[8];
}

// getters
int CanData::getIdentifier()
{
	return m_identifier;
}

double CanData::getTimestamp()
{
	return m_timestamp;
}

string* CanData::getMessageData()
{
	return m_data;
}

int CanData::getDataCount()
{
	return m_dataCount;
}

CanData::DataType CanData::getDataType()
{
	return m_dataType;
}

// setters
void CanData::setIdentifier(unsigned int id)
{
	m_identifier = id;
	setDataType();
}

void CanData::setTimestamp(double ts)
{
	m_timestamp = ts;
}

void CanData::setDataCount(unsigned int dc)
{
	m_dataCount = dc;
}

void CanData::setDataString(string* ds)
{
	m_data = ds;
}

void CanData::setDataType()
{
	switch (m_identifier) 
	{
	case(418384768):
		m_dataType = CanData::DataType::steeringVal;
		break;
	default:
		m_dataType = CanData::DataType::unknownVal;
		break;
	}
}

void CanData::getMessageData(unsigned __int64* data)
{
	*data = m_messageDataU64;
}

/* function converts our can message string array into a single
   U64 which is much easier to manipulate for pulling out 
   individual bitfields and such.
*/
void CanData::convertStringMessageToU64(string* data)
{
	data = m_data;
	// convert data to integer for easier manipulation later
	m_messageDataU64 = 0U;
	for (unsigned int i = 0; i < m_dataCount; i++)
	{
		m_messageDataU64 = m_messageDataU64 | (unsigned __int8)stoi(m_data[i], 0, 16);
		if (i != m_dataCount - 1)
		{
			m_messageDataU64 = m_messageDataU64 << 8;
		}
	}
}

