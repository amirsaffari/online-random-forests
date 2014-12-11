#ifndef ONLINERF_H_
#define ONLINERF_H_

#include "classifier.h"
#include "data.h"
#include "hyperparameters.h"
#include "onlinetree.h"
#include "utilities.h"

class OnlineRF: public Classifier {
public:
    OnlineRF(const Hyperparameters &hp, const int &numClasses, const int &numFeatures, const vector<double> &minFeatRange,
            const vector<double> &maxFeatRange) :
        m_numClasses(&numClasses), m_counter(0.0), m_oobe(0.0), m_hp(&hp) {
        OnlineTree *tree;
        for (int i = 0; i < hp.numTrees; i++) {
            tree = new OnlineTree(hp, numClasses, numFeatures, minFeatRange, maxFeatRange);
            m_trees.push_back(tree);
        }
    }

    ~OnlineRF() {
        for (int i = 0; i < m_hp->numTrees; i++) {
            delete m_trees[i];
        }
    }

    virtual void update(Sample &sample) {
        m_counter += sample.w;

        Result result, treeResult;
        for (int i = 0; i < *m_numClasses; i++) {
            result.confidence.push_back(0.0);
        }

        int numTries;
        for (int i = 0; i < m_hp->numTrees; i++) {
            numTries = poisson(1.0);
            if (numTries) {
                for (int n = 0; n < numTries; n++) {
                    m_trees[i]->update(sample);
                }
            } else {
                treeResult = m_trees[i]->eval(sample);
                if (m_hp->useSoftVoting) {
                    add(treeResult.confidence, result.confidence);
                } else {
                    result.confidence[treeResult.prediction]++;
                }
            }
        }

        if (argmax(result.confidence) != sample.y) {
            m_oobe += sample.w;
        }
    }

    virtual void train(DataSet &dataset) {
        vector<int> randIndex;
        int sampRatio = dataset.m_numSamples / 10;
        for (int n = 0; n < m_hp->numEpochs; n++) {
            randPerm(dataset.m_numSamples, randIndex);
            for (int i = 0; i < dataset.m_numSamples; i++) {
                update(dataset.m_samples[randIndex[i]]);
                if (m_hp->verbose >= 1 && (i % sampRatio) == 0) {
                    cout << "--- Online Random Forest training --- Epoch: " << n + 1 << " --- ";
                    cout << (10 * i) / sampRatio << "%" << endl;
                }
            }
        }
    }

    virtual Result eval(Sample &sample) {
        Result result, treeResult;
        for (int i = 0; i < *m_numClasses; i++) {
            result.confidence.push_back(0.0);
        }

        for (int i = 0; i < m_hp->numTrees; i++) {
            treeResult = m_trees[i]->eval(sample);
            if (m_hp->useSoftVoting) {
                add(treeResult.confidence, result.confidence);
            } else {
                result.confidence[treeResult.prediction]++;
            }
        }

        scale(result.confidence, 1.0 / m_hp->numTrees);
        result.prediction = argmax(result.confidence);
        return result;
    }

    virtual vector<Result> test(DataSet &dataset) {
        vector<Result> results;
        for (int i = 0; i < dataset.m_numSamples; i++) {
            results.push_back(eval(dataset.m_samples[i]));
        }

        double error = compError(results, dataset);
        if (m_hp->verbose >= 1) {
            cout << "--- Online Random Forest test error: " << error << endl;
        }

        return results;
    }

    virtual vector<Result> trainAndTest(DataSet &dataset_tr, DataSet &dataset_ts) {
        vector<Result> results;
        vector<int> randIndex;
        int sampRatio = dataset_tr.m_numSamples / 10;
        vector<double> testError;
        for (int n = 0; n < m_hp->numEpochs; n++) {
            randPerm(dataset_tr.m_numSamples, randIndex);
            for (int i = 0; i < dataset_tr.m_numSamples; i++) {
                update(dataset_tr.m_samples[randIndex[i]]);
                if (m_hp->verbose >= 1 && (i % sampRatio) == 0) {
                    cout << "--- Online Random Forest training --- Epoch: " << n + 1 << " --- ";
                    cout << (10 * i) / sampRatio << "%" << endl;
                }
            }

            results = test(dataset_ts);
            testError.push_back(compError(results, dataset_ts));
        }

        if (m_hp->verbose >= 1) {
            cout << endl << "--- Online Random Forest test error over epochs: ";
            dispErrors(testError);
        }

        return results;
    }

protected:
    const int *m_numClasses;
    double m_counter;
    double m_oobe;
    const Hyperparameters *m_hp;

    vector<OnlineTree*> m_trees;
};

#endif /* ONLINERF_H_ */
