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

clustering, feature selection, spherical k-means, information
retrieval.

========================================================================
How to cite 
========================================================================

An ellipsoidal K-means for document clustering
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

cd < ellkm_directory > && ./ellkm < dataset_dir > > log_run

dataset_dir is a directory structured as follow :
   - dataset_xx.csv
   - dataset_xx.csv
   - dataset_xx.csv
   - dataset_xx.csv
Where xx is a numeric identifier, ellkm will process each dataset
successively in increasing order of their identifiers. Datasets may be
associated with streamed textual data sampled over time.

The actual csv files are `|' separated columns. A file typically
contains a matrix of n documents described as a vector of m
features.

The first line gives headers information which provides descriptive
information about each feature.
The first column identifies each row with an alphanumeric identifier.

========================================================================
Upcoming work 
========================================================================

 - Command line options : sparsity parameter, number of clusters,
   threshold value for objective function. Now this options needs to be
   specified at compile time

 - R binding
