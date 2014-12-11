#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#include <vector>

#include "data.h"

using namespace std;

class Classifier {
public:
    virtual void update(Sample &sample) = 0;
    virtual void train(DataSet &dataset) = 0;
    virtual Result eval(Sample &sample) = 0;
    virtual vector<Result> test(DataSet & dataset) = 0;
    virtual vector<Result> trainAndTest(DataSet &dataset_tr, DataSet &dataset_ts) = 0;

    double compError(const vector<Result> &results, const DataSet &dataset) {
        double error = 0.0;
        for (int i = 0; i < dataset.m_numSamples; i++) {
            if (results[i].prediction != dataset.m_samples[i].y) {
                error++;
            }
        }

        return error / dataset.m_numSamples;
    }

    void dispErrors(const vector<double> &errors) {
        for (int i = 0; i < (int) errors.size(); i++) {
            cout << i + 1 << ": " << errors[i] << " --- ";
        }
        cout << endl;
    }
};

#endif /* CLASSIFIER_H_ */
