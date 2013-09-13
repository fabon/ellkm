#ifndef DATA_TYPES_HXX
# define DATA_TYPES_HXX

namespace kmeans
{
  template<typename T>
  void print_vector(std::vector<T>	&v)
  {
    typename std::vector<T>::iterator it;
    for (it = v.begin();
	 it != v.end();
	 it++)
      std::cout << *it << " ";
    std::cout << std::endl;
  }
};

#endif /* ! DATA_TYPES_HXX */
