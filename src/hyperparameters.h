#ifndef HYPERPARAMETERS_H_
#define HYPERPARAMETERS_H_

#include <string>
using namespace std;

class Hyperparameters
{
 public:
    Hyperparameters();
    Hyperparameters(const string& confFile);

    // Online node
    int numRandomTests;
    int numProjectionFeatures;
    int counterThreshold;
    int maxDepth;

    // Online tree

    // Online forest
    int numTrees;
    int useSoftVoting;
    int numEpochs;

    // Data
    string trainData;
    string testData;

    // Output
    int verbose;
};

#endif /* HYPERPARAMETERS_H_ */
