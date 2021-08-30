/* Header for our main functional code
   Could've made this just an entry point
   to our OOP stuff but stuck with this
   behaving the way it does.  Wasn't too
   long once I had gotten everything else
   written
*/
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "CanData.h"
#include "CanSteeringData.h"
#include "CanUnknownData.h"

using namespace std;

// don't want random magic numbers, so define this here
// this is equivalent to 18F00B80
const int STEERING_MESSAGE_NUMBER = 418384768;

struct wheelData
{
	int axleNum;
	int wheelNum;
};

void chopOffHeader(fstream *);

int getTimestamp(const string, string *);

int getIdentifier(const int, const string, string *);

int getMode(const int, const string, string *);

int getDataBytes(const int, const string, string *);

CanData::DataType getDataType(const int);

bool outputWheelDataToCsv(vector<CanData*>, const int, string);