#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <cmath>
#ifndef WIN32
#include <sys/time.h>
#endif

#include "utilities.h"

using namespace std;

unsigned int getDevRandom() {
    ifstream devFile("/dev/urandom", ios::binary);
    unsigned int outInt = 0;
    char tempChar[sizeof(outInt)];

    devFile.read(tempChar, sizeof(outInt));
    outInt = atoi(tempChar);

    devFile.close();

    return outInt;
}

void randPerm(const int &inNum, vector<int> &outVect) {
    outVect.resize(inNum);
    int randIndex, tempIndex;
    for (int nFeat = 0; nFeat < inNum; nFeat++) {
        outVect[nFeat] = nFeat;
    }
    for (register int nFeat = 0; nFeat < inNum; nFeat++) {
        randIndex = (int) floor(((double) inNum - nFeat) * randDouble()) + nFeat;
        if (randIndex == inNum) {
            randIndex--;
        }
        tempIndex = outVect[nFeat];
        outVect[nFeat] = outVect[randIndex];
        outVect[randIndex] = tempIndex;
    }
}

void randPerm(const int &inNum, const int inPart, vector<int> &outVect) {
    outVect.resize(inNum);
    int randIndex, tempIndex;
    for (int nFeat = 0; nFeat < inNum; nFeat++) {
        outVect[nFeat] = nFeat;
    }
    for (register int nFeat = 0; nFeat < inPart; nFeat++) {
        randIndex = (int) floor(((double) inNum - nFeat) * randDouble()) + nFeat;
        if (randIndex == inNum) {
            randIndex--;
        }
        tempIndex = outVect[nFeat];
        outVect[nFeat] = outVect[randIndex];
        outVect[randIndex] = tempIndex;
    }

    outVect.erase(outVect.begin() + inPart, outVect.end());
}
