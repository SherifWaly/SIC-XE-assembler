//
// Created by Tarek on 5/6/2016.
//

#ifndef ASSEMBLER_OBJECTWRITER_H
#define ASSEMBLER_OBJECTWRITER_H

#include "Includes.h"
#include "AssemblerUtalities.h"

class ObjectWriter {
public:
    const int MAX_RECORD_LENGTH = 60;
    const string SEPARATOR = "";

    ObjectWriter(string fileName);

    void writeHeader(string start, string programName, string programLength);

    /**
     * @brief : append to the current text record
     * @param : opCode -> hex string from the opTable
     * @param : flags: binary string containing all the instruction flags such i, n, x, pc, b, e
     * @param : address: displacement , immediate value or absolute address
    */
    void writeTextRecord(string opCode, string flags, string address);

    /**
     * @brief : start new text record starting from the start parameter
     * @param : start hex string indicating the starting value
     * @param : opCode -> hex string from the opTable
     * @param : flags: binary string containing all the instruction flags such i, n, x, pc, b, e
     * @param : address: displacement , immediate value or absolute address
    */

    void writeTextRecord(string start, string opCode, string flags, string address);

    /**
     * @brief : start new text record starting from the start parameter
     * @param : start hex string indicating the starting value
     * @param : field : hex object to be writen in memory
    */

    void writeTextRecord(string start, string filed);

    /**
     * @brief : append to the text record
     * @param : field : hex object to be writen in memory
    */

    void writeTextRecord(string field);


    /**
     * @brief : write current record
     */
    void writeTextRecord();

    /**
     * @brief : write end text record
     * @param : start : hex start address of the program
     */

    void writeEnd(string start);


    /**
     * @brief : write current record
     * @param : start : start address of the new text record
     */
    void startNewRecord(string start);


    /**
     * @brief : add to modification record list
     * @param : start
                     start address of the modification record of format 4 address
                     relative to start of the program
     */
    void addModificationRecord(string start);

private:
    ofstream out;
    string currentRecord;
    string startAddress = "0000000";
    vector<string> modification;

    void writeModificationRecords();
};


#endif //ASSEMBLER_OBJECTWRITER_H
