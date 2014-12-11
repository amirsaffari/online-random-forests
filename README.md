Online Random Forests
=====================

Amir Saffari <amir@ymer.org>

This is the original implementation of the Online Random Forest algorithm [1]. There is a more recent implementation of this algorithm at https://github.com/amirsaffari/online-multiclass-lpboost which was used in [2].

Read the INSTALL file for build instructions.

Usage:
======
Input arguments:

	 -h | --help : 	 will display this message.
	 -c : 		 path to the config file.

	 --ort : 	 use Online Random Tree (ORT) algorithm.
	 --orf : 	 use Online Random Forest (ORF) algorithm.


	 --train : 	 train the classifier.
	 --test : 	 test the classifier.
	 --t2 : 	 train and test the classifier at the same time.


	Examples:
	 ./Online-Forest -c conf/orf.conf --orf --t2

Config file:
============
All the settings for the classifier are passed via the config file. You can find the
config file in "conf" folder. It is easy to see what are the meanings behind each of
these settings:
Data:
  * trainData = path to the training file
  * testData = path to the test file

Tree:
  * maxDepth = maximum depth for a tree
  * numRandomTests = number of random tests for each node
  * numProjectionFeatures = number of features for hyperplane tests
  * counterThreshold = number of samples to be seen for an online node before splitting

Forest:
  * numTrees = number of trees in the forest
  * numEpochs = number of online training epochs
  * useSoftVoting = boolean flag for using hard or soft voting

Output:
  * savePath = path to save the results (not implemented yet)
  * verbose = defines the verbosity level (0: silence)

Data format:
============
The data formats used is very similar to the LIBSVM file formats. It only need to have
one header line which contains the following information:
\#Samples \#Features \#Classes \#FeatureMinIndex

where

\#Samples: number of samples

\#Features: number of features

\#Classes: number of classes

\#FeatureMinIndex: the index of the first feature used

You can find a few datasets in the data folder, check their header to see some examples.
Currently, there is only one limitation with the data files: the classes should be
labeled starting in a regular format and start from 0. For example, for a 3 class problem
the labels should be in {0, 1, 2} set.

===========
REFERENCES:
===========
[1] Amir Saffari, Christian Leistner, Jakob Santner, Martin Godec, and Horst Bischof,
"On-line Random Forests,"
3rd IEEE ICCV Workshop on On-line Computer Vision, 2009.

[2] Amir Saffari, Martin Godec, Thomas Pock, Christian Leistner, Horst Bischof,
“Online Multi-Class LPBoost“,
Proceedings of IEEE Conference on Computer Vision and Pattern Recognition (CVPR), 2010.
