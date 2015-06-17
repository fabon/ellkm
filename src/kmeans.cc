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

#include <iostream>
#include <fstream>
#include <string>
#include <limits.h>
#include <float.h>
#include <sstream>
#include <set>
#include <iterator>
#include "kmeans.hh"

namespace kmeans
{
  KmeansEllipsoidal::KmeansEllipsoidal(SimilarityFunction		*d,
		 unsigned			k,
		 float				s,
		 unsigned			max_steps,
		 double				min_threshold)
  {
    _k = k;
    _s = s;
    _max_steps = max_steps;
    _min_threshold = min_threshold;

    _current_step = 0;
    _current_run = 0;
    _max_index_centroid = 0;

    _d = d;
  }

  void
  KmeansEllipsoidal::print_data()
  {
    unsigned nrow = 0;
    unsigned ncol = 0;

    nrow = _data.size();
    if (nrow)
      ncol = _data[0].size();

    std::cout << std::endl;
    std::cout << "Data matrix: " << nrow << "x" << ncol << std::endl;
    std::cout << "\t\t";
    foreach(std::string dim, _dims)
      {
	std::cout << dim << " ";
      }
    std::cout << std::endl;

    for (unsigned i = 0;
	 i < _ids.size();
	 i++)
      {
	std::cout << _ids[i] << ":\t";
	for (t_vector_iterator it = _data[i].begin();
	     it != _data[i].end();
	     it++)
	  std::cout << *it << " ";
	std::cout << std::endl;
      }
  }

  bool
  KmeansEllipsoidal::calc_centroid_changement(unsigned		centroid_index,
				   double		&changement)
  {
    if (_old_centroids.empty())
      return false;

    changement = (*_d)(_centroids[centroid_index],
		       _old_centroids[centroid_index]);
    return true;
  }

  void
  KmeansEllipsoidal::calc_data_dims_infos(std::stringstream   &buff)
  {
    t_strings_matrix	aged_dims;
    t_vector		min_vals;
    t_vector		min_vals_for_points;
    unsigned		count_dim = 0;
    unsigned		i,j;
    int			l;

    // init min values for each run
    for (l = _current_run;
	 l > 0 && (unsigned)l >= _current_run - 10;
	 l--)
      {
	min_vals.push_back(1 / pow(2, l));
	min_vals_for_points.push_back(min_vals.back() * _dims.size());
	aged_dims.push_back(t_strings());
      }
    buff << "min vals : ";
    copy(min_vals.begin(), min_vals.end(), std::ostream_iterator<double>(buff, " "));
    buff << std::endl;

    // calc not updated dimensions
    for (j = 0;
	 j < _dims.size();
	 j++)
      {
	for (l = min_vals.size() - 1;
	     l >= 0;
	     l--)
	  {
	    count_dim = 0;
	    for (i = 0;
		 i < _data.size();
		 i++)
	      count_dim += (_data[i][j] <= min_vals[l]  ? 1 : 0);
	    if (count_dim == _data.size())
	      {
		aged_dims[l].push_back(_dims[j]);
		break;
	      }
	  }
      }

    // print results
    buff << "Dims not updated since run : " << std::endl;
    for (j = 0;
	 j < aged_dims.size();
	 j++)
      {
	buff << "t - " <<  (_current_run - j) << " {#" << aged_dims[j].size() << "}: ";
	copy(aged_dims[j].begin(),
	     aged_dims[j].end(),
	     std::ostream_iterator<std::string>(buff, " "));
	buff << std::endl;
      }
    buff << std::endl;
  }

  void
  KmeansEllipsoidal::calc_data_similarity_infos(std::stringstream   &buff)
  {
    double	current_d, min_d, max_d;
    double	acc_d = 0;
    unsigned    count = 0;
    for (unsigned i = 0;
	 i < _data.size();
	 i++)
      for (unsigned j = 0;
	   j < i;
	   j++)
      {
	current_d = (*_d)(_data[i], _data[j]);
	if (min_d > current_d)
	  min_d = current_d;
	if (max_d < current_d)
	  max_d = current_d;
	acc_d += current_d;
	++count;
      }
    buff << "Data similarity infos :";
    buff << "min [" << min_d;
    buff << "] max [" << max_d;
    buff << "] avg. [" << acc_d / (double)count  << "]";
    buff << std::endl;
  }

  void
  KmeansEllipsoidal::calc_global_buzz_words(unsigned		n,
				 std::stringstream	&buff)
  {
    t_vector words = t_vector(_dims.size(), 0);
    unsigned	j = 0;

    for (t_unordered_matrix_iterator it = _centroids.begin();
	 it != _centroids.end();
	 it++)
      for (j = 0;
	   j < _dims.size();
	   j++)
	words[j] += it->second[j];

    calc_n_highest_dims(words,
			n,
			true,
			buff);
  }

  void
  KmeansEllipsoidal::print_objective()
  {
    std::cout << std::endl;
    std::cout << "Step (" << _current_step << ") ";
    std::cout << "Fobj = " << _objective_values.back().back() << " ";
  }

  double
  KmeansEllipsoidal::calc_global_centroids_sparsity()
  {
    double global_sparsity = 0;

    if (_sparsities.empty())
      return 0;

    for (t_map_iterator	sparsity_val = _sparsities.begin();
	 sparsity_val != _sparsities.end();
	 sparsity_val++)
      global_sparsity += sparsity_val->second;
    global_sparsity /= _sparsities.size();

    return global_sparsity;
  }

  static double
  my_sparsity(t_vector	&v)
  {
    double sparsity = 0;
    unsigned j = 0;
    for (j = 0;
	 j < v.size();
	 j++)
      if (v[j] > 0)
	++sparsity;

    return 1 - (sparsity / v.size());
  }

  static unsigned
  my_nonzero(t_vector	&v)
  {
    unsigned cpt = 0;
    for (unsigned j = 0;
	 j < v.size();
	 j++)
      cpt += v[j] > 0;

    return cpt;
  }

  void
  KmeansEllipsoidal::print_snapshot(bool			detailed)
  {
    t_unordered_matrix_iterator	centroid;
    t_unordered_vector_iterator	component;
    t_vector			sparsity_values;
    std::stringstream		buff;

    buff.str("");
    buff << std::endl;
    //calc_data_dims_infos(buff);
    //calc_data_similarity_infos(buff);
    //buff << "Global sparsity [" << calc_global_centroids_sparsity() << "] ";
buff << "** (Active centroids: " << nb_active_centroids() << "/" << _k << ") ";
    buff << "NbPts=" << _data.size() << " ";
    buff << "NbFeats="  << _dims.size() << " " << std::endl;
    buff << "Buzz words:";
    calc_global_buzz_words(DEFAULT_MAX_PRINT_DIMS,
    			   buff);
    buff << std::endl;

    unsigned nb_dims_to_print = detailed ? _dims.size() : DEFAULT_MAX_PRINT_DIMS;
    for (centroid =  _centroids.begin();
	 centroid != _centroids.end();
	 centroid++)
      {
	unsigned i = centroid->first + 1;
	buff << std::endl << "** centroid{"  << i << "/" << _centroids.size() << "} ";
	buff << " NbPts=" << _centroids_sizes[centroid->first] << " ";
	buff << "Fobj(c" << i << ", data)=" << _local_objective_values[centroid->first] << " ";
	buff << "Sparsity=" << my_sparsity(centroid->second) << "% ";
	buff << "NonZeroFeats=" << my_nonzero(centroid->second) << std::endl;

	buff << "feats:";
	calc_n_highest_dims(centroid->second,
			    nb_dims_to_print,
			    true,
			    buff);
	buff << std::endl;
	buff << "lambda:";
	t_vector q(_weights[centroid->first]);
	 for (unsigned j = 0;
	      j <_dims.size();
	      j++)
	   q[j] = pow(q[j], 1.0/_s);
	 //	   q[j] = pow(q[j], 1.0/(_s > 0.0 ? _s : 1.0));
	calc_n_highest_dims(q,
			    nb_dims_to_print,
			    false,
			    buff);
	buff << std::endl;
	// buff << "weights:";
	// calc_n_highest_dims(_weights[centroid->first],
	// 		    nb_dims_to_print,
	// 		    true,
	// 		    buff);
	// buff << std::endl;

	//buff << "Sparsity [" << _sparsities[centroid->first] << "] ";

	// buff << "similarity with last [";
	// double sim = 0;
	// if (false && !calc_centroid_changement(centroid->first,
	// 				       sim))
	//   buff << "n/a";
	// else
	//   buff << sim;
	// buff << "]";
	// buff << std::endl;
	buff << "pts:";
	bool first = true;
	for (unsigned ii = 0;
	     ii < _partition.size();
	     ii++)
	    if (_partition[ii] == centroid->first)
	      {
		buff << (!first ? "," : "") << _ids[ii];
		first = false;
	      }
	buff << std::endl;
      }

    std::cout << buff.str() << std::endl;
  }

  bool
  KmeansEllipsoidal::initialize_centroid_from_input(t_vector input_centroid)
  {
    if (_centroids.size() == _k)
      return false;

    _centroids[_max_index_centroid] = input_centroid;
    normalize_vector(_centroids[_max_index_centroid]);
    update_noweigths(_max_index_centroid);
    ++_max_index_centroid;

    return true;
  }

  bool
  KmeansEllipsoidal::initialize_random()
  {
    t_unordered_matrix_iterator		centroid;
    t_vector				random_centroid;
    t_numbers				shuffled_dims_indices;
    unsigned				nb_dims_to_initialize = 0;
    unsigned				seed = 0;

    seed = time(0);
    //seed = 1324571606;
    //seed = 1325012148;
    srand(seed);
    std::cout << "srand seed value " << seed << std::endl;

    nb_dims_to_initialize = round(_dims.size() - (calc_global_centroids_sparsity() * _dims.size()));
    std::cout << "nb dims to initialize: " << nb_dims_to_initialize << std::endl;
    shuffled_dims_indices = t_numbers(_dims.size());
    random_centroid = t_vector(_dims.size());
    for (unsigned i = 0;
	 _centroids.size() < _k && i < _data.size();
	 i++)
      {
	zero_vector(random_centroid);
	shuffled_dims_indices.clear();
	std::copy(boost::counting_iterator<unsigned>(0),
		  boost::counting_iterator<unsigned>(_dims.size() - 1),
		  std::back_inserter(shuffled_dims_indices));

	std::random_shuffle(shuffled_dims_indices.begin(),
			    shuffled_dims_indices.end());

	for (unsigned j = 0;
	     j < shuffled_dims_indices.size() && j < nb_dims_to_initialize;
	     j++)
	  random_centroid[shuffled_dims_indices[j]] = (double)(rand() % 100) / 100; // fixme le plus bas possible sinon pas bon pour les poids => ce qui est haut ne redecend pas
	normalize_vector(random_centroid);
	for (centroid = _centroids.begin();
	     centroid != _centroids.end();
	     centroid++)
	  if (is_zero_vector(random_centroid) || random_centroid == centroid->second)
	    break;
	if (centroid == _centroids.end()) // not found then ok init
	  {
	    _centroids[_max_index_centroid] = random_centroid;
	    update_noweigths(_max_index_centroid); // set initial weigths uniformely
	    ++_max_index_centroid;
	  }
      }
    return true;
  }

  bool
  KmeansEllipsoidal::initialize_first_k_prototypes()
  {
    t_vector			new_centroid;
    t_unordered_matrix_iterator centroid;
    t_vector			tmp_vec;

    // fixme intialiser sur les points qui vont dans les clusters
    // les plus fragiles ?
    // pour l'instant je prend les premiers points qui arrivent
    // a condition qu'un centroid n'est pas déjà init dessus
    for (unsigned i = 0; // k first data points
	 _centroids.size() < _k && i < _data.size();
	 i++)
      {
	for (centroid = _centroids.begin();
	     centroid != _centroids.end() && !(centroid->second == _data[i]);
	     centroid++)
	  /*nothing*/;

	if (centroid == _centroids.end()) // not found then ok init
	  _centroids[_max_index_centroid++] = _data[i];
      }
    return true;
  }

  bool
  KmeansEllipsoidal::initialize()
  {

    if (!_data.size())
      return false;

    return initialize_random();
    //return initialize_first_k_prototypes();
  }

  struct compare_mapping
  {
    bool operator()(const t_unordered_vector::value_type &elt1,
		    const t_unordered_vector::value_type &elt2) const
    {
      return elt1.second >= elt2.second;
    }
  };

  bool
  KmeansEllipsoidal::calc_n_highest_words(unsigned			n,
			       std::stringstream	&buff)
  {
    t_vector	dim_count;
    unsigned	i = 0;
    unsigned	j = 0;

    dim_count = t_vector(_dims.size(), 0);
    for (i = 0;
	 i < _data.size();
	 i++)
      for (j = 0;
	   j < _data[i].size();
	   j++)
	dim_count[j] += _data[i][j];

    std::set<t_unordered_vector::value_type, compare_mapping> s;
    std::set<t_unordered_vector::value_type, compare_mapping>::iterator s_it;

    for (i = 0;
	 i < dim_count.size();
	 i++)
      s.insert(t_unordered_vector::value_type(_dims[i], dim_count[i]));

    i = 0;
    for (s_it = s.begin();
	 s_it != s.end() && ++i < n;
	 s_it++)
	buff << s_it->first << ":" << s_it->second << " ";
    return true;
  }

  bool
  KmeansEllipsoidal::calc_n_highest_dims(t_vector		&v,
			      unsigned		n,
			      bool		normalized,
			      std::stringstream	&buff)
  {
    t_vector normalized_v;
    double norm = 0;
    unsigned j = 0;

    std::set<t_unordered_vector::value_type, compare_mapping> s;
    std::set<t_unordered_vector::value_type, compare_mapping>::iterator s_it;
    unsigned i = 0;

    if (!normalized)
      normalized_v = v ;
    else
      {
	for (j = 0;
	     j < v.size();
	     j++)
	  norm += pow(v[j], 2);
	norm = pow(norm, (double)1/2);

	for (j = 0;
	     j < v.size();
	     j++)
	  normalized_v.push_back(v[j] / norm);
      }
    for (i = 0;
	 i < v.size();
	 i++)
      s.insert(t_unordered_vector::value_type(_dims[i], normalized_v[i]));

    i = 0;
    for (s_it = s.begin();
	 s_it != s.end() && ++i < n;
	 s_it++)
      if (s_it->second > 0)
	buff << s_it->first << ":" << s_it->second << " ";
    return true;
  }

  bool
  KmeansEllipsoidal::sync_centroids_with_new_data()
  {
    t_unordered_matrix_iterator	centroid;
    t_vector			updated_centroid;
    unsigned			i = 0;
    unsigned			j = 0;

    if (_dims.empty() || _centroids.empty())
      return true;

    if (!_current_run)
      { // get info about first day dims
	std::cout << "ADDED NEW DIMS: ";
	for (t_string_iterator d = _dims.begin();
	     d != _dims.end();
	     d++)
	  std::cout << *d << " ";
	std::cout << std::endl;

	return true; // no need to sync centroids here
      }

    t_unordered_vector added_dims;
    for (centroid = _centroids.begin();
	 centroid != _centroids.end();
	 centroid++)
      {
	updated_centroid.clear();
	/*
	** Add new dims
	*/
	i = 0;
	j = 0;
	while (i < _dims.size())
	  {
	    while (i < _dims.size() &&
		   (j >= _last_dims.size() ||
		    (j < _last_dims.size() &&
		     _dims[i] != _last_dims[j])))
	      {
		updated_centroid.push_back(0);
		added_dims[_dims[i]] = 0;
		++i;
	      }
	    if (i >= _dims.size())
	      break;
	    updated_centroid.push_back(centroid->second[j]);
	    ++j;
	    ++i;
	  }
	centroid->second = updated_centroid;
      }

    if (added_dims.size())
      {
	std::cout << "ADDED NEW DIMS: ";
	for (t_unordered_vector_iterator d = added_dims.begin();
	     d != added_dims.end();
	     d++)
	  std::cout << d->first << " ";
	std::cout << std::endl;
      }

    return true;
  }

  bool
  KmeansEllipsoidal::add_data_point_coord(boost::char_separator<char>	&sparse_delim,
					  std::string			&token,
					  t_vector			&vector)
  {
    boost::tokenizer< boost::char_separator<char> > toks (token, sparse_delim);
    boost::tokenizer< boost::char_separator<char> >::iterator tok = toks.begin();

    size_t index = (size_t)atoi(tok->c_str());

    if (++tok == toks.end())
      {
	vector.push_back(atof(token.c_str()));
	return true;
      }

    if (index <= vector.size())
      return false;

    for (size_t i = vector.size(); i < (index - 1); i++)
      vector.push_back(0.0);

    const std::string *val_str = &(*tok);
    raw_type val = atof(val_str->c_str());

    vector.push_back(val);
    return true;
  }

  bool
  KmeansEllipsoidal::read_data(std::string		data_filepath,
			       const char		*delimiter,
			       bool			headers,
			       bool			user_centroids)
  {
    std::ifstream		f;
    std::string			line;
    std::string			token;
    t_vector			vector;
    bool			first_column = true;
    t_unordered_matrix		input_centroids;
    boost::char_separator<char> sparse_delim(":");

    size_t line_number = 2; //jump the headers
    _ids.clear();
    _data.clear();

    if (!_dims.empty())
      _last_dims = _dims;
    _dims.clear();

    f.open(data_filepath, std::ifstream::in);
    if (!f)
      return false;

    while (!!f)
      {
	getline(f, line);
	if (line.empty())
	  continue;
	boost::char_separator<char> delim(delimiter);
	boost::tokenizer< boost::char_separator<char> > tokens (line, delim);
	if (headers)
	  {
	    first_column = true;
	    foreach(token, tokens)
	      {
		if (first_column)
		  first_column = false;
		else
		  _dims.push_back(token);
	      }
	    //_dims.push_back("BIAS");
	    headers = false;
	    line.clear();
	    continue;
	  }
	first_column = true;
	foreach(token, tokens)
	  {
	    if (first_column)
	      {
		_ids.push_back(token);
		first_column = false;
		continue;
	      }
	    if (!add_data_point_coord(sparse_delim, token, vector))
	      {
		std::cerr << "Input file " << data_filepath << ", l." << line_number;
		std::cerr << ". Please correct the format of data point id " << _ids.back() << std::endl;
		std::cerr << "(see the README for more details about the correct format)" << std::endl;
		return false;
	      }
	  }
	//vector.push_back(1);

	if (vector.size() > _dims.size())
	  {
	    std::cerr << "Input file " << data_filepath << ", l." << line_number;
	    std::cerr << ". Please correct the format of data point id " << _ids.back() << std::endl;
	    std::cerr << "(the header line, " << _dims.size() << " features, does not match with sparse index ";
	    std::cerr << vector.size() << ")" << std::endl;
	    return false;
	  }

	while (vector.size() < _dims.size())
	  vector.push_back(0.0); //sparse format

	if (user_centroids &&
	    initialize_centroid_from_input(vector))
	  std::cout << "added new centroid [" << _centroids.size() << "/" << _k << "]" <<std::endl;
	else
	  _data.push_back(vector);

	//print_vector(vector);
	vector.clear();
	line.clear();
	line_number++;
      }
    f.close();

    return sync_centroids_with_new_data();
  }

  unsigned
  KmeansEllipsoidal::nb_active_centroids()
  {
    return _centroids.size();
  }

  unsigned
  KmeansEllipsoidal::nb_inactive_centroids()
  {
    return _k - _centroids.size();
  }

  bool
  KmeansEllipsoidal::run(t_strings			data_updates_filepaths,
			 bool				user_centroids)
  {
    t_string_iterator	data_update_filepath;
    double	        last_objective_value = 0;
    std::stringstream	buff;

    for (data_update_filepath = data_updates_filepaths.begin();
	 data_update_filepath != data_updates_filepaths.end();
	 data_update_filepath++)
      {
	std::cout << std::endl << "***************************" << std::endl;
	std::cout << "Run (" << _current_run << ")";
	std::cout << std::endl << "***************************" << std::endl;
	if (!read_data(*data_update_filepath,
		  CSV_DELIM,
		  true,
		       user_centroids))
	  return false;
	//print_data();

	buff.str("");
	calc_n_highest_words(DEFAULT_MAX_PRINT_DIMS,
			     buff);
	std::cout << "** HighestFreqWordsInData:" << buff.str() << std::endl;

	if (0 < nb_inactive_centroids())
	  {
	    std::cout << "begin init..."<<std::endl;
	    initialize();
	    std::cout << "end init..." << std::endl;
	  }
	_current_step = 0;
	_objective_values.push_back(t_vector(1, 0)); //init
	do
	  {
	    last_objective_value = _objective_values[_current_run].back();
	    optimize();
	    _objective_values[_current_run].push_back(_objective_value);
	    print_objective();
	    //print_snapshot(false);
	  }
	while (((_objective_value -  last_objective_value) > _min_threshold) &&
	       _current_step++ < _max_steps);
	std::cout << "================> Stop optimization."  << std::endl;
	if (!((_objective_value - last_objective_value) > _min_threshold))
	  {
	    std::cout << "No improvement of Fobj (min threshhold: " << _min_threshold;
	    std::cout << " last Fobj: " << last_objective_value;
	    std::cout << " Fobj : " << _objective_value;
	    std::cout << " delta : " << _objective_value - last_objective_value;
	    std::cout << ")." << std::endl;
	  }

	if (!(_current_step < _max_steps))
	  std::cout << "Max nb steps reached (max nb steps: " << _max_steps << ")." << std::endl;

	print_snapshot(true);
	_old_centroids = _centroids;
	_nb_steps.push_back(_current_step);
	++_current_run;
      }
    return true;
  }

  bool
  KmeansEllipsoidal::update_centroids()
  {
    t_unordered_matrix			new_centroids;
    unsigned				index_centroid = 0;
    unsigned				i = 0;
    unsigned				j = 0;

    if (!_partition.size())
      return true;

    _centroids_sizes.clear();
    for (i = 0;
	 i < _partition.size();
	 i++)
      {
	index_centroid = _partition[i];
	if (new_centroids.end() == new_centroids.find(index_centroid))
	  new_centroids[index_centroid] = t_vector(_dims.size(), 0);

	for (j = 0;
	     j < _dims.size();
	     j++)
	  new_centroids[index_centroid][j] += _data[i][j] * _weights[index_centroid][j];

	if (_centroids_sizes.end() == _centroids_sizes.find(index_centroid))
	  _centroids_sizes[index_centroid] = 1;
	else
	  ++_centroids_sizes[index_centroid];
      }

    for (t_unordered_matrix_iterator centroid = new_centroids.begin();
	 centroid != new_centroids.end();
	 centroid++)
      normalize_vector(centroid->second);

    _centroids = new_centroids;
    return true;
  }

  bool
  KmeansEllipsoidal::update_distances()
  {
    t_unordered_matrix_iterator		centroid;
    t_vector				point;
    t_vector				old_partition;
    double				max_similarity = 0;
    double				current_distance = 0;
    unsigned				index_centroid = 0;

    _objective_value = 0;
    _local_objective_values.clear();
    old_partition = _partition;
    _partition.clear(); // fixme store them for community analysis

    foreach (point, _data)
      {
	max_similarity = 0;
	for (centroid = _centroids.begin();
	     centroid != _centroids.end();
	     centroid++)
	  {
	    current_distance = (*_d)(point,
				     centroid->second,
				     _weights[centroid->first]);
	    if (max_similarity < current_distance)
	      {
		max_similarity = current_distance;
		index_centroid = centroid->first;
	      }
	  }

	_partition.push_back(index_centroid);
	if (_local_objective_values.end() == _local_objective_values.find(index_centroid))
	  _local_objective_values[index_centroid] = 0;
	_local_objective_values[index_centroid] += max_similarity;
      }
    for (t_map_iterator it = _local_objective_values.begin();
	 it != _local_objective_values.end();
	 it++)
      _objective_value += _local_objective_values[it->first];
    return true;
  }

  bool
  KmeansEllipsoidal::update_noweigths(int	index_centroid)
  {
    t_unordered_matrix_iterator	centroid;
    unsigned			m = 0;
    double			val = 0;

    m = _dims.size();
    val = pow(1 / (double)m, _s); // useless to raise to the power here
    //val = 1 / (double)m;
    if (0 > index_centroid)
      for (centroid = _centroids.begin();
	   centroid != _centroids.end();
	   centroid++)
	_weights[centroid->first] = t_vector(m, val);
    else
      _weights[index_centroid] = t_vector(m, val);

    return true;
  }

  bool
  KmeansEllipsoidal::optimize()
  {
    if ((_current_run > 0 && _current_step == 0)) // transition from past day partition
      {
	// re-calc centroids with uniform weigths
	std::cout << "transitioning from past day..."<<std::endl;
	std::cout << "uniform weigths..."<<std::endl;
	update_noweigths();
	std::cout << "centroids coordinates with uniform weights and past day partition..."<<std::endl;
	//update_centroids();
      }

    std::cout << "update distances..." <<std::endl;
    update_distances();
    std::cout << "update centroids..."<<std::endl;
    update_centroids();
    std::cout << "update weights..."<<std::endl;
    update_weigths();
    std::cout << "done." << std::endl;
    return true;
  }

  bool
  KmeansEllipsoidal::update_weigths()
  {
    t_unordered_matrix_iterator	centroid;
    t_vector			point;
    unsigned			m = 0;
    unsigned			n = 0;
    unsigned			j = 0;
    unsigned			i = 0;
    double			overall_similarity = 0;
    t_vector			cluster_data;
    t_vector			per_dimension_similarity;
    double			coeff_power = 1 / ((double)WEIGHT_SKEWNESS - 1);
    //double			data_norm = 0;

    m = _dims.size();
    n = _data.size();
    for (centroid = _centroids.begin();
  	 centroid != _centroids.end();
  	 centroid++)
      {
	std::cout << "c[" << centroid->first << "] ";
	cluster_data = t_vector(m, 0);
	for (i = 0;
	     i < n;
	     i++)
	  if (_partition[i] == centroid->first)
	    {
	      if (cluster_data.empty())
		cluster_data = t_vector(m, 0);
	      for (j = 0;
		   j < m;
		   j++)
		cluster_data[j] += _data[i][j];
	    }

	_d->store(cluster_data,
		  centroid->second,
		  per_dimension_similarity);

	overall_similarity = 0;
	for (j = 0;
	     j < m;
	     j++)
	  {
	    if (per_dimension_similarity[j] > 0)
	      {
		per_dimension_similarity[j] = 1 / pow(per_dimension_similarity[j], coeff_power);
		overall_similarity += per_dimension_similarity[j];
	      }
	  }

	double acc = 0;
	double h = 0;
	double sparsity = 0;
	for (j = 0;
	     j < m;
	     j++)
	  per_dimension_similarity[j] *= 1 / overall_similarity;

	for (j = 0;
	     j < m;
	     j++)
	  {
	    acc += per_dimension_similarity[j];
	    if (per_dimension_similarity[j] == 0)
	      ++sparsity;
	    if (per_dimension_similarity[j] > 0)
	      h -= per_dimension_similarity[j] * (log(per_dimension_similarity[j])/ log(m));
	    per_dimension_similarity[j] = pow(per_dimension_similarity[j], _s);
	  }

	std::cout << "sum [" << acc << "] ";
	std::cout << "entropy [" << h << "] ";
	std::cout << "sparsity [" << sparsity / m << "] " << std::endl;
	_weights[centroid->first] = per_dimension_similarity;
      }
    return true;
  }
};
