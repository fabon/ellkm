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

run in a terminal:
```
cd < ellkm_directory > && ./configure && make
```

========================================================================
Examples
========================================================================

run in a terminal:
```
 bash ./example.sh
```

========================================================================
Description of the packaged binary
========================================================================

```
./ellkm < data_dir > < sparsity parameter (real \\in [0,0.5], s=0 yields spkm) > < K > [ user_centroids 1 | 0] > log_run
```

dataset_dir is a directory structured as follow :
   - dataset_1.csv
   - dataset_2.csv
   - ...
   - dataset_N.csv

where dataset_i.csv, i \in [1..N] are N datasets to be clustered
sequentially in a streaming scenario. Ellkm will process each dataset
successively in increasing order of their indices: each run results in a partitionning
that evolve with the dynamics of the input data and demonstrates the evolution of
communities discovered over time.

========================================================================
Data format
========================================================================

Input csv files must be formatted as `,' separated columns, they describe a matrix X of n documents
embedded in a m-dimensional feature spaces.

The first column is the identifier of a data sample. All other columns described the data in feature space as
follows:
```
	ids,feat1,feat2,feat3,feat4,feat5
	1,0,0,1,0.5,0
	2,0.05,1,0,0.5,0
	...
	400,0.3,0,0,0.125,0.32
	...
```
Data can all also be given in sparse format:
```
	ids,feat1,feat2,feat3,feat4,feat5
	1,2:0,1,3:0.5
	2,1:0.05,2:1,4:0.5
	...
	400,1:0.3,4:0.125,5:0.32
	...
```
where sparse indices start at index 1 and are given in increasing order.

The number of columns of the N input files need not be equal.
Ellkm handle the addition of new features over time.

========================================================================
What has changed
========================================================================
- v2.0)
  - input data can be read in sparse format and in scientific notation
  - vectors are now outputed in sparse format
  - more readable output
  - weight vectors are now accessible on the standard output
- v1.0)
  - User centroids : it is now possible to provide ellkm with initial centroids, e.g
   when K clusters make up a partition and ellkm is invoked with an additional `1`
   command line argument, then the first K lines of `data_dir` are used to initialise
   the K centroids.
  - a different seed (given by `time(0)`) is now used for each run
  - a bug led to the double insertion of the last point from each csv dataset.

========================================================================
Upcoming work
========================================================================
 - command line options : maximum number of iteration (default to 20), minimum
   threshold value for objective function (default to 10^(-9)). Now these options needs to be
   specified at compile time (-D MAX_STEPS=XX -D MIN_THRESHOLD=XX)
 - include gap procedure as described in [dzogang et al., 2012]
 - R binding
