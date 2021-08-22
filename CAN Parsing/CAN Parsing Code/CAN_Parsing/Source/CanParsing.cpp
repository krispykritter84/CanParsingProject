/* CanParsing.cpp : This file contains the 'main' function.Program execution begins and ends there.
*  This part is kinda basic, but it takes a single argument, which is the path to a CAN datalog
*  and parses the whole file.  Steering data gets its own object, where all other CAN data just 
*  gets dumped into a 'unknown' object.  Could extend this later to add any other data of interest.
*/
#include "../Headers/CanParsing.h"

using namespace std;

int main(int argc, char ** argv)
{
    // verify we got an argument (first arg is self path, so we need to make sure we got one 
    // more than that.
    if (argc != 3)
    {
        cout << "This program requires a path to a CAN logger file as the first input," << endl;
        cout << "and the desired output file name as the second input." << endl;
        cout << "You've provided either too few or too many args." << endl;
        return 0;  // quit now, no need to go any farther
    }
    // if we got an arg, see if we can open this file
    fstream canFileStream;
    canFileStream.open(argv[1], ios::in);
    if (canFileStream.is_open())
    {
        cout << "File found." << endl;
    }
    else
    {
        cout << "I can't find the file you specified.  Verify path is correct." << endl;
        return 0;
    }
    int lineCount = 0;
    int steerAxleMsgCnt = 0;
    vector<CanData*> canDataVector;
     
    // first, get passed the header:
    chopOffHeader(&canFileStream);

    // tell people we're working on it...
    cout << "Processing...";

    // then, start parsing the file
    while (canFileStream.good())
    {
        lineCount++;
        // keep doing things so they know we've not out to lunch
        if (lineCount % 1000 == 0)
            cout << ".";
        // gonna need all these variables
        string parsedTimestamp;
        string parsedIdentifier;
        string parsedMode;
        string line;
        int stringPosition = 0;

        getline(canFileStream, line);
        // get our timestamp
        stringPosition = getTimestamp(line, &parsedTimestamp);
        //cout << "Timestamp is " << parsedTimestamp << endl;

        // get our module identifier
        stringPosition = getIdentifier(stringPosition, line, &parsedIdentifier);
        //cout << "Identifier is " << parsedIdentifier << endl;

        // get our mode
        stringPosition = getMode(stringPosition, line, &parsedMode);
        //cout << "Mode is " << parsedMode << endl;

        // get our data bytes
        string* data = new string[8];
        int dataByteCount = getDataBytes(stringPosition, line, data);
        CanData::DataType thisCanDataType = getDataType(stoi(parsedIdentifier, 0, 16));

        // figure which type we are
        if (thisCanDataType == CanData::DataType::steeringVal)
        {
            //CanSteeringData* newCanEntry = new CanSteeringData(stoi(parsedIdentifier, 0, 16), stof(parsedTimestamp), dataByteCount, thisCanDataType, data);
            canDataVector.push_back(new CanSteeringData(stoi(parsedIdentifier, 0, 16), stof(parsedTimestamp), dataByteCount, thisCanDataType, data));
        }
        else
        {
            canDataVector.push_back(new CanUnknownData(stoi(parsedIdentifier, 0, 16), stof(parsedTimestamp), dataByteCount, thisCanDataType, data));
        }

        // next thing to do is add the data to the can steering data class
        if (canFileStream.peek() == EOF)
        {
            break;
        }
    }
    // close file for reasons
    canFileStream.close();

    // tell people all the cool things we did
    cout << endl << "Parsed " << canDataVector.size() << " messages." << endl;

    // chop path out of input filename
    string filePathAndName = argv[1];
    // assume windows first
    size_t endOfPath = filePathAndName.find_last_of("/\\");
    string fullPathToFile = filePathAndName.substr(0, endOfPath+1);

    // output stuff to file
    return outputWheelDataToCsv(canDataVector, STEERING_MESSAGE_NUMBER, fullPathToFile + argv[2]) ? 0 : -1;
}

// this just removes the top few lines from the datalog
// had to assume they'd always be there, so I look for the
// // that makes the version number as the last line of the
// header
void chopOffHeader(fstream * fs)
{
    string line;
    while (true)
    {
        getline(*fs, line);
        if (line[0] == '/')
        {
            break;
        }
    }
}

// get our timestamp data
// return is current position in input line
int getTimestamp(string inputLine, string * outputLine)
{
    // skip passed first set of spaces
    int i = 0;
    while (inputLine[i] == ' ')
    {
        i++;
    }
    // found start of time stamp, now find end of timestamp
    int j = i;
    while (inputLine[j] != ' ')
    {
        j++;
    }
    // now return subsring from start of timestamp to end of timestamp minus start
    *outputLine = inputLine.substr(i, j-i);
    return j; // current position in line
}

// figure out our message number, keeping just the hex characters
// return is current position in input line
int getIdentifier(int pos, string inputLine, string * outputLine)
{
    // cheating, but move 4 places to the right
    // we'll assume this format is guaranteed for now
    int i = pos + 4;
    int j = i;
    int end = 0;
    string longId;
    // skip to the next space
    while (inputLine[j] != ' ')
    {
        j++;
    }
    // get our value
    longId = inputLine.substr(i, j - i);
    // remove trailing x, if it exists, and set our output
    if (longId[j - i - 1] == 'x')
    {
        *outputLine = longId.substr(0, j - i - 1);
    }
    else
    {
        *outputLine = longId.substr(0, j - i);
    }
    //set outputLine value to our substring
    //cout << "Ident " << *outputLine << endl;
    //cout << "STOI " << stoi(*outputLine, 0, 16) << endl;
    return j; // current position in input line
}

// figures out if this message is a receive (Rx) or transmit (Tx)
// there weren't any Tx's in our data but it's easy enough to 
// account for.
int getMode(int pos, string inputLine, string * outputLine)
{
    // skip passed the spaces
    int i = pos;
    while (inputLine[i] == ' ')
    {
        i++;
    }
    // found start of mode now find end of mode
    int j = i;
    while (inputLine[j] != ' ')
    {
        j++;
    }
    // now return subsring from start of timestamp to end of timestamp minus start
    *outputLine = inputLine.substr(i, j - i);
    return j;  // current position in input line
}

// returns CAN data in a string array via a pointer
// returns number of data bytes directly as int
int getDataBytes(int pos, string inputLine, string * outputDataBytes)
{
    // start by getting data bytes count
    // cheating again, advancing position by five
    int i = pos + 5;
    int dataByteCount = stoi(inputLine.substr(i, 1));
    // update our position
    i += 2;
    // Parse out the data bits
    for (int j = 0; j < dataByteCount; j++)
    {
        outputDataBytes[j] = inputLine.substr(i, 2);
        i += 3;
    }
    return dataByteCount;
}

// figure out what type of message we got from message header data
CanData::DataType getDataType(int id)
{
    switch (id)
    {
    case(STEERING_MESSAGE_NUMBER):
        return CanData::DataType::steeringVal;
    default:
        return CanData::DataType::unknownVal;
    }
}

// open a datafile and output the messages of interest in CSV format
// because .... it's easy.
bool outputWheelDataToCsv(vector<CanData*> cd, int messageIdentifier, string filename)
{
    size_t position = filename.find(".csv");
    if (position == string::npos)
    {
        filename = filename.append(".csv");
    }
    fstream opFilestream;
    opFilestream.open(filename, ios::out);
    // output title row
    opFilestream << "Axle Number,Wheel Number,Timestamp,Wheel Angle,\n";
    // count how many wheels are on this thing
    vector<wheelData> wd;
    for (unsigned int i = 0; i < cd.size(); i++)
    {
        // could do this a little more elegant but this works for now
        if (cd[i]->getIdentifier() == STEERING_MESSAGE_NUMBER)
        {
            bool addWheel = true;
            CanData* canData = cd[i];
            CanSteeringData* csd = dynamic_cast<CanSteeringData*>(canData);
            for (unsigned int j = 0; j < wd.size(); j++)
            {
                if (wd[j].axleNum == csd->getAxleNumber() && wd[j].wheelNum == csd->getWheelNumber())
                {
                    addWheel = false;
                }
            }
            if (addWheel)
            {
                wheelData nwd = wheelData();
                nwd.axleNum = csd->getAxleNumber();
                nwd.wheelNum = csd->getWheelNumber();
                wd.push_back(nwd);
            }
        }
    }
    for (unsigned int i = 0; i < wd.size(); i++)
    {
        int axleNum = wd[i].axleNum;
        int wheelNum = wd[i].wheelNum;
        int messageCount = 0;
        for (unsigned int j = 0; j < cd.size(); j++)
        {
            // could do this a little more elegant but this works for now
            if (cd[j]->getIdentifier() == STEERING_MESSAGE_NUMBER)
            {
                CanData* canData = cd[j];
                CanSteeringData* csd = dynamic_cast<CanSteeringData*>(canData);
                if (csd->getAxleNumber() == axleNum && csd->getWheelNumber() == wheelNum)
                {
                    messageCount++;
                    opFilestream << csd->getAxleNumber() << "," << csd->getWheelNumber() << "," << csd->getTimestamp() << 
                        "," << csd->getWheelAngle() << ",\n";
                }
            }
        }
        // print how many message were output and to what file
        cout << "Wrote " << messageCount << " steering messages for axle " << axleNum << 
            " and wheel " << wheelNum << endl;
    }
    opFilestream.close();
    return true; // can use this for output error checking}
}