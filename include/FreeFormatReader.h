#ifndef FREEFORMATREADER_H
#define FREEFORMATREADER_H
#include "InputReader.h"

class FreeFormatReader : public InputReader {
public:
    FreeFormatReader(string fileName): InputReader(fileName) {}
    bool hasNextLine() override;
protected:
    void parse(string line);
private:
};

#endif // FREEFORMATREADER_H
