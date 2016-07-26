#ifndef FIXEDFORMATREADER_H
#define FIXEDFORMATREADER_H

#ifndef INCLUDES_H
#include "Includes.h"
#endif // INCLUDES_H

#ifndef INPUTREADER_H
#include "InputReader.h"
#endif // INPUTREADER_H

class FixedFormatReader : public InputReader {
public:
    FixedFormatReader(string fileName): InputReader(fileName) {};
    bool hasNextLine() override;
private:
    bool trailSpaces(string & s, int n);
};

#endif // FIXEDFORMATREADER_H
