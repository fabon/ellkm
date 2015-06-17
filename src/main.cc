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
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <boost/algorithm/string/predicate.hpp>
#include "kmeans.hh"

static int
usage()
{
  std::cout << "./ellkm < data_dir > < sparsity parameter (real number \\in [0,0.5], s=0 yields spkm) > < K (number of clusters) > [ user centroids 0 | 1 ]" << std::endl;
  return 2;
}

static bool
compare_filenames (const std::string &s1, const std::string &s2)
{
  size_t pos1, pos2;
  std::string id;
  int	id1, id2;


  pos1 = s1.find_last_of("_");
  pos2 = s1.find_last_of(".");
  id = s1.substr(pos1 + 1, pos2 - (pos1 + 1));
  //std::cout << id << std::endl;
  id1 = atoi(id.c_str());

  pos1 = s2.find_last_of("_");
  pos2 = s2.find_last_of(".");
  id = s2.substr(pos1 + 1, pos2 - (pos1 + 1));
  //std::cout << id << std::endl;
  id2 = atoi(id.c_str());

  return (id1 < id2);
}

static bool
my_strtol(const char	*s,
	  long		&val)
{
  char *endptr = 0;

  errno = 0;
  val = strtol(s,
	       &endptr,
	       10);
  if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
      || (errno != 0 && val == 0))
    {
      perror("strtol");
      return false;
    }
  if ((!val && s[0] != '0'))
      return false;
  return true;
}

static bool
my_strtof(const char	*s,
	  float		&val)
{
  char *endptr = 0;

  errno = 0;
  val = strtof(s,
	       &endptr);

  if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
      || (errno != 0 && val == 0))
    {
      perror("strtol");
      return false;
    }
  if ((!val && s[0] != '0'))
      return false;
  return true;
}

int main(int argc,
	 char **argv)
{
  kmeans::t_strings		data_paths;
  struct dirent			*entry = 0;
  DIR				*dir = 0;
  std::string			filename;

  const char			*dir_name = 0;
  float				s_value = WEIGHT_SKEWNESS;
  long				k_value = K;

  if (argc > 5)
    return usage();

  bool user_centroids =
    argc == 5 && argv[4] && *argv[4] - '0' == 1;

  dir_name = argv[1];
  if ( !my_strtof(argv[2], s_value) ||
       !my_strtol(argv[3], k_value))
    return usage();
  kmeans::copyright();

  std::cout << "s value [" << s_value << "]" << std::endl;
  std::cout << "number of clusters [" << k_value << "]" << std::endl;
  std::cout << "user given centroids [" << user_centroids << "]" << std::endl;

  if (!(dir = opendir(argv[1])))
    return usage();

  while((entry = readdir(dir)))
    {
      filename = entry->d_name;
      if (boost::algorithm::ends_with(filename, ".csv"))
	{
	  filename = dir_name;
	  filename += dir_name[strlen(dir_name) - 1] != '/' ? "/" : "";
	  filename += entry->d_name;
	  data_paths.push_back(filename);
	}
    }
  sort(data_paths.begin(),
       data_paths.end(),
       compare_filenames);

  std::cout << "files to process: " << std::endl;
  copy(data_paths.begin(),
       data_paths.end(),
       std::ostream_iterator<std::string>(std::cout,"\n"));

  kmeans::SimilarityFunction		*cosine_similarity = new kmeans::CosineSimilarity(2);
  kmeans::KmeansEllipsoidal		*ellkm = 0;

  ellkm = new kmeans::KmeansEllipsoidal(cosine_similarity,
					k_value,
					s_value);
  if (!ellkm->run(data_paths, user_centroids)) {
    return 3;
  }

  delete cosine_similarity;
  delete ellkm;
}
