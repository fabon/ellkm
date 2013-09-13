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
#include "kmeans.hh"

static int
usage()
{
  std::cout << "./ellkm < data_dir >" << std::endl;
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
  id1 = atoi(id.c_str());

  pos1 = s2.find_last_of("_");
  pos2 = s2.find_last_of(".");
  id = s2.substr(pos1 + 1, pos2 - (pos1 + 1));
  id2 = atoi(id.c_str());

  return (id1 < id2);
}

int main(int argc,
	 char **argv)
{
  kmeans::t_strings		data_paths;
  struct dirent			*entry = 0;
  DIR				*dir = 0;
  std::string			filename;

  if (argc != 2)
    return usage();

  if (!(dir = opendir(argv[1])))
    return usage();

  while((entry = readdir(dir)))
    {
      filename = entry->d_name;
      if (std::string::npos != filename.find(".csv"))
	{
	  filename = argv[1];
	  filename += "/";
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

  kmeans::SimilarityFunction    *cosine_similarity = new kmeans::CosineSimilarity(2);
  kmeans::Kmeans		*ellkm = 0;

  ellkm = new kmeans::KmeansEllipsoidal(cosine_similarity);

  ellkm->run(data_paths);

  delete cosine_similarity;
  delete ellkm;
}
