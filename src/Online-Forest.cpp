#define GMM_USES_BLAS

#include <cstdlib>
#include <iostream>
#include <time.h>
#include <string>
#include <string.h>
#include <libconfig.h++>

#include "data.h"
#include "onlinetree.h"
#include "onlinerf.h"

using namespace std;
using namespace libconfig;

typedef enum {
    ORT, ORF
} CLASSIFIER_TYPE;

//! Prints the interface help message
void help() {
    cout << endl;
    cout << "OnlineForest Classification Package:" << endl;
    cout << "Input arguments:" << endl;
    cout << "\t -h | --help : \t will display this message." << endl;
    cout << "\t -c : \t\t path to the config file." << endl << endl;
    cout << "\t --ort : \t use Online Random Tree (ORT) algorithm." << endl;
    cout << "\t --orf : \t use Online Random Forest (ORF) algorithm." << endl;
    cout << endl << endl;
    cout << "\t --train : \t train the classifier." << endl;
    cout << "\t --test : \t test the classifier." << endl;
    cout << "\t --t2 : \t train and test the classifier at the same time." << endl;
    cout << endl << endl;
    cout << "\tExamples:" << endl;
    cout << "\t ./Online-Forest -c conf/orf.conf --orf --train --test" << endl;
}

//! Returns the time (ms) elapsed between two calls to this function
double timeIt(int reset) {
    static time_t startTime, endTime;
    static int timerWorking = 0;

    if (reset) {
        startTime = time(NULL);
        timerWorking = 1;
        return -1;
    } else {
        if (timerWorking) {
            endTime = time(NULL);
            timerWorking = 0;
            return (double) (endTime - startTime);
        } else {
            startTime = time(NULL);
            timerWorking = 1;
            return -1;
        }
    }
}

int main(int argc, char *argv[]) {
    // Parsing command line
    string confFileName;
    int classifier = -1, doTraining = false, doTesting = false, doT2 = false, inputCounter = 1;

    if (argc == 1) {
        cout << "\tNo input argument specified: aborting." << endl;
        help();
        exit(EXIT_SUCCESS);
    }

    while (inputCounter < argc) {
        if (!strcmp(argv[inputCounter], "-h") || !strcmp(argv[inputCounter], "--help")) {
            help();
            return EXIT_SUCCESS;
        } else if (!strcmp(argv[inputCounter], "-c")) {
            confFileName = argv[++inputCounter];
        } else if (!strcmp(argv[inputCounter], "--ort")) {
            classifier = ORT;
        } else if (!strcmp(argv[inputCounter], "--orf")) {
            classifier = ORF;
        } else if (!strcmp(argv[inputCounter], "--train")) {
            doTraining = true;
        } else if (!strcmp(argv[inputCounter], "--test")) {
            doTesting = true;
        } else if (!strcmp(argv[inputCounter], "--t2")) {
            doT2 = true;
        } else {
            cout << "\tUnknown input argument: " << argv[inputCounter];
            cout << ", please try --help for more information." << endl;
            exit(EXIT_FAILURE);
        }

        inputCounter++;
    }

    cout << "OnlineMCBoost Classification Package:" << endl;

    if (!doTraining && !doTesting && !doT2) {
        cout << "\tNothing to do, no training, no testing !!!" << endl;
        exit(EXIT_FAILURE);
    }

    if (doT2) {
        doTraining = false;
        doTesting = false;
    }

    // Load the hyperparameters
    Hyperparameters hp(confFileName);

    // Creating the train data
    DataSet dataset_tr, dataset_ts;
    dataset_tr.loadLIBSVM(hp.trainData);
    if (doT2 || doTesting) {
        dataset_ts.loadLIBSVM(hp.testData);
    }

    // Calling training/testing
    switch (classifier) {
    case ORT: {
        OnlineTree model(hp, dataset_tr.m_numClasses, dataset_tr.m_numFeatures, dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);
        if (doT2) {
            timeIt(1);
            model.trainAndTest(dataset_tr, dataset_ts);
            cout << "Training/Test time: " << timeIt(0) << endl;
        }
        if (doTraining) {
            timeIt(1);
            model.train(dataset_tr);
            cout << "Training time: " << timeIt(0) << endl;
        } else if (doTesting) {
            timeIt(1);
            model.test(dataset_ts);
            cout << "Test time: " << timeIt(0) << endl;
        }
        break;
    }
    case ORF: {
        OnlineRF model(hp, dataset_tr.m_numClasses, dataset_tr.m_numFeatures, dataset_tr.m_minFeatRange, dataset_tr.m_maxFeatRange);
        if (doT2) {
            timeIt(1);
            model.trainAndTest(dataset_tr, dataset_ts);
            cout << "Training/Test time: " << timeIt(0) << endl;
        }
        if (doTraining) {
            timeIt(1);
            model.train(dataset_tr);
            cout << "Training time: " << timeIt(0) << endl;
        }
        if (doTesting) {
            timeIt(1);
            model.test(dataset_ts);
            cout << "Test time: " << timeIt(0) << endl;
        }
        break;
    }
    }

    return EXIT_SUCCESS;
}
