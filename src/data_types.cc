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
