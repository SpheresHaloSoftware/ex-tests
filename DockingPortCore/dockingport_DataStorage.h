#ifndef DP_DATA_STORAGE_H
#define DP_DATA_STORAGE_H

// C++ libraries
#include <fstream>
#include <string>
#include <dirent.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>

// OpenCV
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;


class dockingport_DataStorage
{

	char testname[100];
	char GPFpath[200];
	char GSstoragepath[200];
	char timeOfTeststart[200];

	char timetagStorageFileName[250];
//	char leftImageStorageName[250];
//	char rightImageStorageName[250];


	cv::Mat storageImg, storageImgUnrect;

	void makeDirectory(char newDirectory[200]);

public:
	ofstream timetagStorageFile;
	int numsaved;
	char singleImageStorageName[250];
	char singleImageStorageNameUnrect[250];
	char imagestoragepath[250];


	bool autoImageStorage, unrectifiedImageStorage;
	pthread_mutex_t storage_mutex;
	cv::Mat singleStorageImg;
	cv::Mat singleStorageImgUnrect;

	dockingport_DataStorage() {

		numsaved = 0;
		autoImageStorage = true;
		unrectifiedImageStorage = false;
	}

	void initDataStorage(char _testname[100], char _testprojectpath[200], int imgWidth, int imgHeight);

	void saveTimestampedImages(double imagetimestamp);

	void shutdownDataStorage();

	string getGSdatastoragePath();

	string getGPFpath();

	string newGSDataDirectory(char newDirName[200]);

	string getTimeOfTestStart();

	int getNumSaved();
};

#endif
