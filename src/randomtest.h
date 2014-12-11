#ifndef RANDOMTEST_H_
#define RANDOMTEST_H_

#include "data.h"
#include "utilities.h"

class RandomTest {
public:
    RandomTest() {

    }

    RandomTest(const int &numClasses) :
        m_numClasses(&numClasses), m_trueCount(0.0), m_falseCount(0.0) {
        for (int i = 0; i < numClasses; i++) {
            m_trueStats.push_back(0.0);
            m_falseStats.push_back(0.0);
        }
        m_threshold = randomFromRange(-1, 1);
    }

    RandomTest(const int &numClasses, const double featMin, const double featMax) :
        m_numClasses(&numClasses), m_trueCount(0.0), m_falseCount(0.0) {
        for (int i = 0; i < numClasses; i++) {
            m_trueStats.push_back(0.0);
            m_falseStats.push_back(0.0);
        }
        m_threshold = randomFromRange(featMin, featMax);
    }

    void updateStats(const Sample &sample, const bool decision) {
        if (decision) {
            m_trueCount += sample.w;
            m_trueStats[sample.y] += sample.w;
        } else {
            m_falseCount += sample.w;
            m_falseStats[sample.y] += sample.w;
        }
    }

    double score() {
        double totalCount = m_trueCount + m_falseCount;

        // Split Entropy
        double p, splitEntropy = 0.0;
        if (m_trueCount) {
            p = m_trueCount / totalCount;
            splitEntropy -= p * log2(p);
        }
        if (m_trueCount) {
            p = m_trueCount / totalCount;
            splitEntropy -= p * log2(p);
        }

        // Prior Entropy
        double priorEntropy = 0.0;
        for (int i = 0; i < *m_numClasses; i++) {
            p = (m_trueStats[i] + m_falseStats[i]) / totalCount;
            if (p) {
                priorEntropy -= p * log2(p);
            }
        }

        // Posterior Entropy
        double trueScore = 0.0, falseScore = 0.0;
        if (m_trueCount) {
            for (int i = 0; i < *m_numClasses; i++) {
                p = m_trueStats[i] / m_trueCount;
                if (p) {
                    trueScore -= p * log2(p);
                }
            }
        }
        if (m_falseCount) {
            for (int i = 0; i < *m_numClasses; i++) {
                p = m_falseStats[i] / m_falseCount;
                if (p) {
                    falseScore -= p * log2(p);
                }
            }
        }
        double posteriorEntropy = (m_trueCount * trueScore + m_falseCount * falseScore) / totalCount;

        // Information Gain
        return (2.0 * (priorEntropy - posteriorEntropy)) / (priorEntropy * splitEntropy + 1e-10);
    }

    pair<vector<double> , vector<double> > getStats() {
        return pair<vector<double> , vector<double> > (m_trueStats, m_falseStats);
    }

protected:
    const int *m_numClasses;
    double m_threshold;
    double m_trueCount;
    double m_falseCount;
    vector<double> m_trueStats;
    vector<double> m_falseStats;
};

class HyperplaneFeature: public RandomTest {
public:
    HyperplaneFeature() {

    }

    HyperplaneFeature(const int &numClasses, const int &numFeatures, const int &numProjFeatures, const vector<double> &minFeatRange,
            const vector<double> &maxFeatRange) :
        RandomTest(numClasses), m_numProjFeatures(&numProjFeatures) {
        randPerm(numFeatures, numProjFeatures, m_features);
        fillWithRandomNumbers(numProjFeatures, m_weights);

        // Find min and max range of the projection
        double minRange = 0.0, maxRange = 0.0;
        for (int i = 0; i < numProjFeatures; i++) {
            minRange += minFeatRange[m_features[i]] * m_weights[i];
            maxRange += maxFeatRange[m_features[i]] * m_weights[i];
        }

        m_threshold = randomFromRange(minRange, maxRange);
    }

    void update(Sample &sample) {
        updateStats(sample, eval(sample));
    }

    bool eval(Sample &sample) {
        double proj = 0.0;
        for (int i = 0; i < *m_numProjFeatures; i++) {
            proj += sample.x[m_features[i]] * m_weights[i];
        }

        return (proj > m_threshold) ? true : false;
    }

private:
    const int *m_numProjFeatures;
    vector<int> m_features;
    vector<double> m_weights;
};

#endif /* RANDOMTEST_H_ */
