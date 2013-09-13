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

#ifndef	DATA_TYPES_HH
#define DATA_TYPES_HH

#include <math.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <boost/foreach.hpp>

#define	raw_type	double

#define foreach	BOOST_FOREACH

namespace kmeans
{
  typedef	std::vector<std::string>			t_strings;
  typedef	t_strings::iterator				t_string_iterator;

  typedef	std::vector<t_strings>				t_strings_matrix;
  typedef	t_strings_matrix::iterator			t_strings_matrix_iterator;

  typedef	std::vector<raw_type>				t_vector;
  typedef	t_vector::iterator				t_vector_iterator;

  typedef	std::map<std::string, raw_type>			t_unordered_vector;
  typedef	t_unordered_vector::iterator			t_unordered_vector_iterator;

  typedef	std::map<unsigned, raw_type>			t_map;
  typedef	t_map::iterator					t_map_iterator;

  typedef	std::vector<t_vector>				t_matrix;
  typedef	t_matrix::iterator				t_matrix_iterator;

  typedef       std::map<unsigned, t_vector>			t_unordered_matrix;
  typedef	t_unordered_matrix::iterator			t_unordered_matrix_iterator;

  typedef       std::map<unsigned, t_unordered_vector>		t_data_frame;
  typedef	t_data_frame::iterator				t_data_frame_iterator;

  typedef	std::vector<unsigned>				t_numbers;
  typedef	t_numbers::iterator				t_numbers_iterator;

  typedef	std::map<unsigned,unsigned>			t_unordered_numbers;
  typedef	t_unordered_numbers::iterator			t_unordered_numbers_iterator;

  template<typename T>
  void print_vector(std::vector<T>	&v);

  void unordered_vector_to_vector(t_unordered_vector	&v,
  				  t_strings		&order,
  				  t_vector		&res);

  void	zero_vector(t_vector	&v);
  bool	is_zero_vector(t_vector	&v);
  bool	is_invalid_vector(t_vector	&v);
  void  normalize_vector(t_vector &v);
};


#include "data_types.hxx"

#endif /* ! DATA_TYPES_HH */
