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

#include "data_types.hh"

namespace kmeans
{
  void
  unordered_vector_to_vector(t_unordered_vector	&v,
			     t_strings		&order,
			     t_vector		&res)
  {
    std::string dim;

    res.clear();
    foreach (dim, order)
      res.push_back(v[dim]);
  }

  void
  normalize_vector(t_vector &v)
  {
    double vector_norm = 0;

    for (t_vector_iterator it = v.begin();
	 it != v.end();
	 it++)
      vector_norm += pow(*it, (double)2);
    vector_norm = pow(vector_norm, (double)1/2);

    for (t_vector_iterator it = v.begin();
	 it != v.end();
	 it++)
      *it /= vector_norm;
  }

  void
  zero_vector(t_vector	&v)
  {
    for (t_vector_iterator it = v.begin();
	 it != v.end();
	 it++)
      *it = 0;
  }

  bool
  is_zero_vector(t_vector	&v)
  {
    for (t_vector_iterator it = v.begin();
	 it != v.end();
	 it++)
      if (*it > 0)
	return false;
    return true;
  }

  bool
  is_invalid_vector(t_vector	&v)
  {
    for (t_vector_iterator it = v.begin();
	 it != v.end();
	 it++)
      if (isnan(*it) || isinf(*it))
	return true;
    return false;
  }

};
