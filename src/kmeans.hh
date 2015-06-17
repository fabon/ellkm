/**
 ** Copyright 2013 Fabon Dzogang
 **
 ** This file is part of ellkm.

 ** ellkm is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** any later version.

 ** ellkm is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.

 ** You should have received a copy of the GNU General Public License
 ** along with ellkm.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef K_MEANS_HH
# define K_MEANS_HH

#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/iterator/counting_iterator.hpp>

#include "data_types.hh"
#include "similarity_function.hh"

#define K				15
#define MIN_THRESHOLD			0.000000001
#define MAX_STEPS			20
#define WEIGHT_SKEWNESS			0.14
#define DEFAULT_MAX_PRINT_DIMS		12
#define DEFAULT_MAX_PRINT_CENTROID_DIMS	112
#define CSV_DELIM			","

namespace kmeans
{
  inline void copyright()
  {
    std::cout <<
      "ellkm  Copyright (C) 2013  Fabon Dzogang\n"
      "This program comes with ABSOLUTELY NO WARRANTY.\n"
      "This is free software, and you are welcome to redistribute it\n"
      "under certain conditions; run `ellkm help' for details.\n"
	      << std::endl;
  }

  class KmeansEllipsoidal
  {
  public:
    KmeansEllipsoidal(SimilarityFunction		*d,
	   unsigned			k = K,
	   float			s = WEIGHT_SKEWNESS,
	   unsigned			max_steps = MAX_STEPS,
	   double			min_threshold = MIN_THRESHOLD);

    ~KmeansEllipsoidal() {}

    bool		read_data(std::string	data_filepath,
				  const char    *delimiter = ",",
				  bool		headers = true,
				  bool		user_centroids = false);

    bool		run(t_strings		data_updates_filepaths,
			    bool		user_centroids);

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
    bool		add_data_point_coord(boost::char_separator<char>	&sparse_delim,
					     std::string			&token,
					     t_vector				&vector);
    bool		initialize_random();
    bool		initialize_centroid_from_input(t_vector input_centroid);
    bool		initialize_first_k_prototypes();
    bool		initialize();
    unsigned		nb_active_centroids();
    unsigned		nb_inactive_centroids();
    bool		optimize();
    bool		update_distances();
    bool		update_centroids();
    bool		update_weigths();
    bool		update_noweigths(int		index_centroid = -1);

    bool		sync_centroids_with_new_data();
    double		calc_global_centroids_sparsity();


    SimilarityFunction	*_d;
    unsigned		_k;
    float		_s;
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
};


#endif /* ! K_MEANS_HH */
