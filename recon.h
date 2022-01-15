#ifndef RECON_H
#define RECON_H

#define CERES_FOUND 1

#include <opencv2/sfm.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>

#include <iostream>
#include <fstream>

using namespace cv;
using namespace cv::sfm;

static int getdir(const std::string Filename, std::vector<String> &files){

	std::ifstream myfile(Filename.c_str());
	if (!myfile.is_open()) {
		std::cout << "Unable to read file: " << Filename << std::endl;
		exit(0);
	} else {;
		size_t found = Filename.find_last_of("/\\");
		std::string line_str, path_to_file = Filename.substr(0, found);
		while ( getline(myfile, line_str) )
			files.push_back(path_to_file+std::string("/")+line_str);
	}
	return 1;
}

std::vector<Vec3f> PointCloud(const std::string Filename, float F, float Cx, float Cy) {

	std::vector<String> filePaths;
	getdir(Filename, filePaths);\

	Matx33d K = Matx33d( F, 0, Cx,
			     0, F, Cy,
			     0, 0,  1);

	bool isProjective = true;

	std::vector<Mat> rsEst, tsEst, pointsEstimated;
	reconstruct(filePaths, rsEst, tsEst, K, pointsEstimated, isProjective);

	std::vector<Vec3f> cloud;

	for (int i = 0; i < pointsEstimated.size(); ++i)
		cloud.push_back(Vec3f(pointsEstimated[i]));

	return cloud;
}

#endif // RECON_H
