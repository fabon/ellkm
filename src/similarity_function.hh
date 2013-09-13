#ifndef SIMILARITY_FUNCTION_HH
# define SIMILARITY_FUNCTION_HH

#include <algorithm>
#include <math.h>

namespace kmeans
{
  struct SimilarityFunction
  {
    virtual double	operator()(const t_vector	&x,
				   const t_vector	&y) = 0;

    virtual double	operator()(const t_vector	&x,
				   const t_vector	&y,
				   const t_vector	&w) = 0;

    virtual bool	store(const t_vector	&x,
			      const t_vector	&y,
			      t_vector		&res) = 0;
    virtual ~SimilarityFunction() {}
  };

  struct CosineSimilarity : public SimilarityFunction
  {
    CosineSimilarity(unsigned	p = 2)
    {
      _p = p;
    }

    virtual ~CosineSimilarity() {}

    virtual double
    operator()(const t_vector	&x,
    	       const t_vector	&y)
    {
      unsigned	i = 0;
      double	norme = 0;
      unsigned  min_size = 0;

      min_size = std::min(x.size(), y.size());
      while (i < min_size)
    	{
    	  norme += x[i] * y[i];
    	  ++i;
    	}
      return norme;
    }

    virtual double
    operator()(const t_vector	&x,
	       const t_vector	&y,
	       const t_vector	&w)
    {
      unsigned	i = 0;
      double	norme = 0;
      unsigned  min_size = 0;

      min_size = std::min(x.size(), y.size());
      while (i < min_size)
	{
	  norme += w[i] * x[i] * y[i];
	  ++i;
	}

      return norme;
    }

    virtual bool
    store(const t_vector	&x,
	  const t_vector	&y,
	  t_vector		&res)
    {
      unsigned	i = 0;
      unsigned  min_size = 0;

      res.clear();
      min_size = std::min(x.size(), y.size());
      while (i < min_size)
	{
	  res.push_back(x[i] * y[i]);
	  ++i;
	}

      return true;
    }

  protected:
    unsigned	_p;
  };

  struct PDistance : public SimilarityFunction
  {
    PDistance(unsigned	p = 2)
    {
      _p = p;
    }

    virtual double
    operator()(const t_vector	&x,
	       const t_vector	&y,
	       const t_vector	&w)
    {
      double t = x[0];
      t = y[0];
      t = w[0];

      return t;
    };

    virtual double
    operator()(const t_vector	&x,
	       const t_vector	&y)
    {
      unsigned	i = 0;
      double	d = 0;
      unsigned  min_size = 0;

      min_size = std::min(x.size(), y.size());
      while (i < min_size)
	{
	  d += pow(fabs(x[i] - y[i]), _p);
	  ++i;
	}

      d = pow(d, (double)1 / _p);
      return d;
    }

    virtual bool
    store(const t_vector	&x,
	  const t_vector	&y,
	  t_vector		&res)
    {
      double t = x[0];
      t = y[0];
      res.clear();
      res[0] = t;
      return true;
    }

  protected:
    unsigned	_p;
  };

  struct ManhattanDistance : public SimilarityFunction
  {
    virtual double
      operator()(const t_vector	&x,
		 const t_vector	&y)
    {
      PDistance d(1);
      return d(x,y);
    }

    virtual double
    operator()(const t_vector	&x,
	       const t_vector	&y,
	       const t_vector	&w)
    {
      double t = x[0];
      t = y[0];
      t = w[0];

      return t;
    };

    virtual bool
    store(const t_vector	&x,
	  const t_vector	&y,
	  t_vector		&res)
    {
      double t = x[0];
      t = y[0];
      res.clear();
      res[0] = t;
      return true;
    }

  };

  struct EuclideanDistance : public SimilarityFunction
  {
    virtual double
      operator()(const t_vector	&x,
		 const t_vector	&y)
    {
      PDistance d(2);
      return d(x,y);
    }
    virtual double
    operator()(const t_vector	&x,
	       const t_vector	&y,
	       const t_vector	&w)
    {
      double t = x[0];
      t = y[0];
      t = w[0];
      return t;
    };

    virtual bool
    store(const t_vector	&x,
	  const t_vector	&y,
	  t_vector		&res)
    {
      double t = x[0];
      t = y[0];
      res.clear();
      res[0] = t;
      return true;
    }

  };
}

#endif /* ! SIMILARITY_FUNCTION_HH */
