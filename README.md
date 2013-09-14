========================================================================
Presentation
========================================================================

We propose an extension of the spherical K-means algorithm to deal
with settings where the number of data points is largely inferior to
the number of dimensions. We assume the data to lie in local and dense
regions of the original space and we propose to embed each cluster
into its specific ellipsoid. A new objective function is introduced,
analytical solutions are derived and exploited for the update of both
the centroids and the associated ellipsoids. Furthermore, a study on
the complexity of this algorithm highlights that it is of same order
as the regular K-means algorithm.

========================================================================
Keywords
========================================================================

clustering, high-dimension, feature selection, spherical k-means,
textmining, information retrieval, C++.

========================================================================
How to cite
========================================================================

[dzogang et al., 2012] An ellipsoidal K-means for document clustering
12th IEEE International Conference on Data Mining (ICDM'2012) - pp. 221--230
Dzogang, Fabon and Marsala, Christophe and Lesot, Marie-Jeanne and
Rifqi, Maria

========================================================================
Installation
========================================================================

cd < ellkm_directory > && ./configure && make

========================================================================
Binary description
========================================================================

./ellkm < data_dir > < sparsity parameter (real \\in [0,0.5], s=0 yields spkm) > < K > > log_run

dataset_dir is a directory structured as follow :
   - dataset_true_version.csv
   - dataset_randomized_version_1.csv
   - ...
   - dataset_randomized_version_N.csv

Where dataset_true_version.csv is the original dataset to be clustered
and dataset_randomized_version_xx.csv are N randomized version, for
example obtained by randomly switching columns in the true data. As
described in [dzogang et al., 2012] ellkm will process each dataset
successively in increasing order of their identifiers. The resulting
objective values may be used for different values of the sparsity
parameter in order to compute a gap score for heuristically choosing
an appropriate number of relevant features.

The actual csv files are `|' separated columns. A file typically
contains a matrix of n documents described as a vector of m
features.

The first line gives headers information which provides descriptive
information about each feature.

The first column identifies each row with an alphanumeric identifier.

========================================================================
Upcoming work
========================================================================

 - Command line options : maximum number of iterator (default to 20), minimum
   threshold value for objective function (default to 10^(-9)). Now this options needs to be
   specified at compile time (-D MAX_STEPS=XX -D MIN_THRESHOLD=XX)
 - include gap procedure as described in [dzogang et al., 2012]
 - R binding
