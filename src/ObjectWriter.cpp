//
// Created by Tarek on 5/6/2016.
//

#include "ObjectWriter.h"

ObjectWriter::ObjectWriter(string fileName) {
    out.open(fileName);
}

void ObjectWriter::writeHeader(string start, string programName, string programLength) {
    while (programName.length() < 6) {
        programName = programName + ' ';
    }
    start = autalities::normalize(start, 6);
    programLength = autalities::normalize(programLength, 6);
    startAddress = start;
    currentRecord = "";
    out << "H" << programName << SEPARATOR << autalities::toUp(start) << SEPARATOR << autalities::toUp(programLength) << "\n";
}

void ObjectWriter::writeTextRecord(string opCode, string flags, string address) {
    int answer = autalities::hexToInteger(opCode);
    answer <<= 4;
    answer |= autalities::binToInteger(flags);
    string result = autalities::intToHex(answer, 3);
    result += address;
    writeTextRecord(result);
}

void ObjectWriter::writeTextRecord(string start, string opCode, string flags, string address) {
    startNewRecord(start);
    writeTextRecord(opCode, flags, address);
}

void ObjectWriter::writeTextRecord(string start, string filed) {
    startNewRecord(start);
    writeTextRecord(filed);
}

void ObjectWriter::writeEnd(string start) {
    startNewRecord("");
    writeModificationRecords();
    out << "E" << autalities::normalize(autalities::toUp(start), 6) << "\n";
    out.close();
}

void ObjectWriter::writeTextRecord(string field) {
    if (currentRecord.length() + field.size() > MAX_RECORD_LENGTH) {
        // write record
        writeTextRecord();
        int previousStart = autalities::hexToInteger(startAddress);
        startAddress = autalities::intToWord(currentRecord.length() + previousStart);
        currentRecord = "";
    }
    currentRecord += field;
}

void ObjectWriter::writeTextRecord() {
    if (!currentRecord.empty()) {
        out << "T" << autalities::toUp(startAddress) << SEPARATOR << autalities::toUp(autalities::intToByte(currentRecord.length() / 2)) << SEPARATOR;
        out << autalities::toUp(currentRecord) << "\n";
    }
}

void ObjectWriter::startNewRecord(string start) {
    //write record
    if (!currentRecord.empty()) {
        writeTextRecord();
        currentRecord = "";
    }
    startAddress = start;
}

void ObjectWriter::addModificationRecord(string start) {
    modification.push_back(autalities::normalize(autalities::toUp(start), 6) + SEPARATOR + "05");
}

void ObjectWriter::writeModificationRecords() {
    for(auto mod : modification) {
        out << "M" << mod << "\n";
    }
}
