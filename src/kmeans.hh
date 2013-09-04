#ifndef K_MEANS_HH
# define K_MEANS_HH

#include "data_types.hh"
#include "similarity_function.hh"

#define K				15
//#define MIN_THRESHOLD			0.0000001
#define MIN_THRESHOLD			0.000000001
#define MAX_STEPS			20
 // #define WEIGHT_SKEWNESS			0
// #define WEIGHT_SKEWNESS		0.14
 #define WEIGHT_SKEWNESS			0.2
//#define DEFAULT_MAX_PRINT_DIMS	800000
#define DEFAULT_MAX_PRINT_DIMS		12
#define DEFAULT_MAX_PRINT_CENTROID_DIMS	112

namespace kmeans
{
  class Kmeans
  {
  public:
    Kmeans(SimilarityFunction		*d,
	   unsigned			k = K,
	   unsigned			max_steps = MAX_STEPS,
	   double			min_threshold = MIN_THRESHOLD);

    virtual ~Kmeans() {}

    bool		read_data(std::string	data_filepath,
				  const char    *delimiter = ",",
				  bool		headers = true);

    bool		run(t_strings		data_updates_filepaths);

    void		print_objective();
    void		print_snapshot(bool				detailed = false);
    void		print_data();
    void		calc_data_dims_infos(std::stringstream   &buff);

    bool		calc_centroid_changement(unsigned		centroid_index,
						 double			&changement);
    void		calc_data_similarity_infos(std::stringstream		&buff);
    void		calc_global_buzz_words(unsigned			n,
					       std::stringstream	&buff);
    bool		calc_n_highest_words(unsigned		n,
					     std::stringstream	&buff);
    bool		calc_n_highest_dims(t_vector		&v,
					    unsigned		n,
					    bool		normalized,
					    std::stringstream	&buff);

  protected:
    bool		initialize_random();
    bool		initialize_first_k_prototypes();
    bool		initialize();
    unsigned		nb_active_centroids();
    unsigned		nb_inactive_centroids();
    virtual bool	optimize() = 0;
    bool		update_distances();
    bool		update_centroids();
    bool		update_noweigths(int		index_centroid = -1);

    bool		sync_centroids_with_new_data();
    double		calc_global_centroids_sparsity();


    SimilarityFunction	*_d;
    unsigned		_k;
    unsigned		_max_steps;
    double		_min_threshold;

    t_matrix		_data;
    t_strings		_dims;
    t_unordered_matrix	_weights;
    t_strings		_last_dims;
    t_strings		_ids;
    t_unordered_matrix	_centroids;
    t_unordered_matrix	_old_centroids;
    t_unordered_matrix	_centroids_distances;
    t_unordered_numbers	_centroids_sizes;
    t_map		_sparsities;
    t_map		_centroids_deplacements;
    unsigned		_max_index_centroid;

    t_matrix		_objective_values;
    double		_objective_value;
    t_map		_local_objective_values;
    t_vector		_nb_steps;
    unsigned		_current_step;
    unsigned		_current_run;

    t_vector		_partition;
  };

  class KmeansSpherical : public Kmeans
  {
  public:
    KmeansSpherical(SimilarityFunction		*d,
		unsigned			k = K,
		unsigned			max_steps = MAX_STEPS,
		double				min_threshold = MIN_THRESHOLD) :
      Kmeans(d,
	     k,
	     max_steps,
	     min_threshold)
    {}

    virtual ~KmeansSpherical() {}

    virtual bool optimize();

  protected:
    virtual bool update_weigths()
    {
      return update_noweigths();
    }
  };

  class KmeansSphericalWeightedExp : public KmeansSpherical
  {
  public:
    KmeansSphericalWeightedExp(SimilarityFunction	*d,
			       unsigned			k = K,
			       unsigned			max_steps = MAX_STEPS,
			       double			min_threshold = MIN_THRESHOLD) :
      KmeansSpherical(d,
		      k,
		      max_steps,
		      min_threshold)
    {}

    virtual ~KmeansSphericalWeightedExp() {}

  protected:
    virtual bool update_weigths();

  };

  class KmeansSphericalWeightedEntropy : public KmeansSpherical
  {
  public:
    KmeansSphericalWeightedEntropy(SimilarityFunction	*d,
				   unsigned		k = K,
				   unsigned		max_steps = MAX_STEPS,
				   double		min_threshold = MIN_THRESHOLD) :
      KmeansSpherical(d,
		      k,
		      max_steps,
		      min_threshold)
    {}
  protected:
    virtual bool update_weigths();

  };

};


#endif /* ! K_MEANS_HH */
