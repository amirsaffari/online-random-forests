#include <iostream>

#include "onlinenode.h"

using namespace std;

void OnlineNode::update(Sample &sample) {
    m_counter += sample.w;
    m_labelStats[sample.y] += sample.w;

    if (m_isLeaf) {
        // Update online tests
        for (int i = 0; i < m_hp->numRandomTests; i++) {
            m_onlineTests[i].update(sample);
        }

        // Update the label
        m_label = argmax(m_labelStats);

        // Decide for split
        if (shouldISplit()) {
            m_isLeaf = false;

            // Find the best online test
            int maxIndex = 0;
            double maxScore = -1e10, score;
            for (int i = 0; i < m_hp->numRandomTests; i++) {
                score = m_onlineTests[i].score();
                if (score > maxScore) {
                    maxScore = score;
                    maxIndex = i;
                }
            }
            m_bestTest = m_onlineTests[maxIndex];
            m_onlineTests.clear();

            if (m_hp->verbose >= 4) {
                cout << "--- Splitting node --- best score: " << maxScore;
                cout << " by test number: " << maxIndex << endl;
            }

            // Split
            pair<vector<double> , vector<double> > parentStats = m_bestTest.getStats();
            m_rightChildNode = new OnlineNode(*m_hp, *m_numClasses, *m_numFeatures, *m_minFeatRange, *m_maxFeatRange, m_depth + 1,
                    parentStats.first);
            m_leftChildNode = new OnlineNode(*m_hp, *m_numClasses, *m_numFeatures, *m_minFeatRange, *m_maxFeatRange, m_depth + 1,
                    parentStats.second);
        }
    } else {
        if (m_bestTest.eval(sample)) {
            m_rightChildNode->update(sample);
        } else {
            m_leftChildNode->update(sample);
        }
    }
}
