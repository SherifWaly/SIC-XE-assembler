#include <getopt.h>
#include <PassOne.h>
#include <FixedFormatReader.h>

#ifndef INCLUDES_H
#include <Includes.h>
#endif // INCLUDES_H

#include "PassTwo.h"

void run(bool isFixed, string sourceName, string outputName) {
    PassOne *p;
    string interFile = sourceName+"_list.txt";
    cout << "source path: \"" << sourceName << "\"" << endl;
    cout << "intermediate file: \"" << interFile << "\"" << endl;
    cout << "output file: \"" << outputName << "\"" << endl;
    if(isFixed) {
        p = new PassOne(new FixedFormatReader(sourceName), interFile);
    } else {
        p = new PassOne(sourceName, interFile);
    }
    p->pass();
    cout << "\n***************************\n\n";
    if (p->getErrorsCounter() == 0) {
        PassTwo *p2 = new PassTwo(interFile, p->getSymTable(), p->getLiteralPool(), p->getprogrammLength(), outputName);
        p2->pass();
    }
}

void print_usage();

int main(int argc, char *argv[]) {
    bool isFixed = false;
    char *outputPath = NULL;
    char *sourcePath = NULL;

    int c;
    while ((c = getopt (argc, argv, "hxo:")) != -1) {
        if (c == 'x') {
            isFixed = true;
        } else if (c == 'o') {
            outputPath = optarg;
        } else if(c == '?') {
            if (optopt == 'o')
                fprintf (stderr, "Option -%c requires an output path.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf (stderr,
                         "Unknown option character `\\x%x'.\n",
                         optopt);
            print_usage();
            return 1;
        } else {
            print_usage();
            abort ();
        }
    }
    if (optind == argc - 1) {
        sourcePath = argv[optind];
    } else if (optind < argc) {
        fprintf (stderr, "too many arguments\n");
        print_usage();
        return 1;
    } else {
        fprintf (stderr, "source file path is messing\n");
        print_usage();
        return 1;
    }
    string source(sourcePath);
    string output;
    if (outputPath == NULL) {
        output = source + "_output.txt";
    } else {
        output = *(new string(outputPath));
    }
    run(isFixed, source, output);
    return 0;
}

void print_usage() {
    printf(
        "sic assembler pass1\n"
        "Usage: pass1 <sourcefile path> options [-x][-o outputPath]\n"
        "Options:\n"
        "\t[-x]             fixed format flag.\n"
        "\t[-o outputPath]  specified output path (default called <sourcefile>_out.txt).\n"
        "\t-h               print help.\n"
    );
}
