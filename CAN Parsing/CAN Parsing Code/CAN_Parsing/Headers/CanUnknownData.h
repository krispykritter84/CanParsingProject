/* Skeleton for adding other data of interest */
#pragma once
#include "CanData.h"

class CanUnknownData : public CanData {
public:
	CanUnknownData(int, double, int, CanData::DataType, string*);

private:
	// we don't do anything with these data yet

};
