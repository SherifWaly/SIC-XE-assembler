#ifndef INTERMEDIATEREADER_H
#define INTERMEDIATEREADER_H

#include "FreeFormatReader.h"


class IntermediateReader : public FreeFormatReader {
public:
    IntermediateReader(string fileName) : FreeFormatReader(fileName) {};
    bool hasNextLine();
    string getLocator() {return locator;}
protected:
private:
    string locator;
};

#endif // INTERMEDIATEREADER_H
