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

int getTimestamp(string, string *);

int getIdentifier(int, string, string *);

int getMode(int, string, string *);

int getDataBytes(int, string, string *);

CanData::DataType getDataType(int);

bool outputWheelDataToCsv(vector<CanData*>, int, string);