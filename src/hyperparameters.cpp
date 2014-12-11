#include <iostream>
#include <libconfig.h++>

#include "hyperparameters.h"

using namespace std;
using namespace libconfig;

Hyperparameters::Hyperparameters(const string& confFile) {
    cout << "Loading config file: " << confFile << " ... ";

    Config configFile;
    configFile.readFile(confFile.c_str());

    // Node/Tree
    maxDepth = configFile.lookup("Tree.maxDepth");
    numRandomTests = configFile.lookup("Tree.numRandomTests");
    numProjectionFeatures = configFile.lookup("Tree.numProjectionFeatures");
    counterThreshold = configFile.lookup("Tree.counterThreshold");

    // Forest
    numTrees = configFile.lookup("Forest.numTrees");
    numEpochs = configFile.lookup("Forest.numEpochs");
    useSoftVoting = configFile.lookup("Forest.useSoftVoting");

    // Data
    trainData = (const char *) configFile.lookup("Data.trainData");
    testData = (const char *) configFile.lookup("Data.testData");

    // Output
    verbose = configFile.lookup("Output.verbose");

    cout << "Done." << endl;
}
