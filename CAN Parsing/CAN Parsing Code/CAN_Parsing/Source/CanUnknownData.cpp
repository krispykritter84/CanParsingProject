#include "../Headers/CanUnknownData.h"
#include "../Headers/CanData.h"

CanUnknownData::CanUnknownData(int id, double ts, int dc, CanData::DataType dt, string* data)
{
	setIdentifier(id);
	setTimestamp(ts);
	setDataCount(dc);
	setDataString(data);
	convertStringMessageToU64(data);
}
