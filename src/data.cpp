#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "data.h"

using namespace std;

void DataSet::findFeatRange() {
    double minVal, maxVal;
    for (int i = 0; i < m_numFeatures; i++) {
        minVal = m_samples[0].x[i];
        maxVal = m_samples[0].x[i];
        for (int n = 1; n < m_numSamples; n++) {
            if (m_samples[n].x[i] < minVal) {
                minVal = m_samples[n].x[i];
            }
            if (m_samples[n].x[i] > maxVal) {
                maxVal = m_samples[n].x[i];
            }
        }

        m_minFeatRange.push_back(minVal);
        m_maxFeatRange.push_back(maxVal);
    }
}

void DataSet::loadLIBSVM(string filename) {
    ifstream fp(filename.c_str(), ios::binary);
    if (!fp) {
        cout << "Could not open input file " << filename << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Loading data file: " << filename << " ... " << endl;

    // Reading the header
    int startIndex;
    fp >> m_numSamples;
    fp >> m_numFeatures;
    fp >> m_numClasses;
    fp >> startIndex;

    // Reading the data
    string line, tmpStr;
    int prePos, curPos, colIndex;
    m_samples.clear();

    for (int i = 0; i < m_numSamples; i++) {
        wsvector<double> x(m_numFeatures);
        Sample sample;
        resize(sample.x, m_numFeatures);
        fp >> sample.y; // read label
        sample.w = 1.0; // set weight

        getline(fp, line); // read the rest of the line
        prePos = 0;
        curPos = line.find(' ', 0);
        while (prePos <= curPos) {
            prePos = curPos + 1;
            curPos = line.find(':', prePos);
            tmpStr = line.substr(prePos, curPos - prePos);
            colIndex = atoi(tmpStr.c_str()) - startIndex;

            prePos = curPos + 1;
            curPos = line.find(' ', prePos);
            tmpStr = line.substr(prePos, curPos - prePos);
            x[colIndex] = atof(tmpStr.c_str());
        }
        copy(x, sample.x);
        m_samples.push_back(sample); // push sample into dataset
    }

    fp.close();

    if (m_numSamples != (int) m_samples.size()) {
        cout << "Could not load " << m_numSamples << " samples from " << filename;
        cout << ". There were only " << m_samples.size() << " samples!" << endl;
        exit(EXIT_FAILURE);
    }

    // Find the data range
    findFeatRange();

    cout << "Loaded " << m_numSamples << " samples with " << m_numFeatures;
    cout << " features and " << m_numClasses << " classes." << endl;
}
