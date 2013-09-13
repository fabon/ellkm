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
