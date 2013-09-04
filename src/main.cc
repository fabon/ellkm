#include <iostream>
#include <dirent.h>
#include "kmeans.hh"

static int
usage()
{
  std::cout << "./kmeans_transmitters < data_dir >" << std::endl;
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
  kmeans::Kmeans		*kmeans_exp = 0;

  kmeans_exp = new kmeans::KmeansSphericalWeightedExp(cosine_similarity);
  // kmeans_batch = new kmeans::KmeansBatch(norme1);
  // kmeans_batch = new kmeans::KmeansBatch(distance2);

  kmeans_exp->run(data_paths);
  //kmeans_entropy->run(data_paths);

  delete cosine_similarity;
  delete kmeans_exp;
}