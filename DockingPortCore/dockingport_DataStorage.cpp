#include "dockingport_DataStorage.h"

void dockingport_DataStorage::makeDirectory(char newDirectory[200]) {
	char newDirectorySystemCmd[200];
	int dummy2;
	sprintf(newDirectorySystemCmd,"sudo mkdir %s", newDirectory);
	dummy2 = system(newDirectorySystemCmd);

	// make sure, that write permissions to that directory are given
	sprintf(newDirectorySystemCmd, "sudo chmod 777 %s", newDirectory);
	dummy2 = system(newDirectorySystemCmd);
}

void dockingport_DataStorage::initDataStorage(char _testname[100], char _GPFpath[200], int imgWidth, int imgHeight) {

	sprintf(testname, "%s", _testname);
	sprintf(GPFpath,"%s", _GPFpath);

	char newDirectory[200];
	char newSubdirectory[200];
	char newDirectorySystemCmd[200];

	pthread_mutex_init(&this->storage_mutex, NULL);

	cv::Size size(imgWidth*2 , imgHeight);
	this->storageImg.create( size, CV_8UC1 );
	this->singleStorageImg = this->storageImg( cv::Rect(0, 0, imgWidth, imgHeight) );

	this->storageImgUnrect.create( size, CV_8UC1 );
	this->singleStorageImgUnrect = this->storageImgUnrect( cv::Rect(0, 0, imgWidth, imgHeight) );


	// check if general Results Directory exists, if not, create it
//	sprintf(newDirectory, "%s/Results", GPFpath);
	sprintf(newDirectory, "%s/Results", GPFpath);
    DIR *resultsDir;
    resultsDir = opendir (newDirectory);
    if (resultsDir == NULL)
    {
		// make new RESULTS directory for whole Program = Group
    	this->makeDirectory(newDirectory);
    }

	// check if Results Directory for current testproject exists, if not, create it
//	sprintf(newDirectory, "%s/Results/%s", GPFpath, testname);
	sprintf(newDirectory, "%s/Results/%s", GPFpath, testname);
	resultsDir = opendir (newDirectory);
    if (resultsDir == NULL)
    {
		// make new RESULTS directory for currently running testproject
    	this->makeDirectory(newDirectory);
    }

	// get current UTC time for generating new results directory for currently running test
	  time_t rawtime;
	  struct tm * ptm;
	  time ( &rawtime );
	  ptm = gmtime ( &rawtime ); // gives the time in UTC
	  sprintf (this->timeOfTeststart, "%04d_%02d_%02d_%02d_%02d_%02d", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

		// make new directory first
		sprintf(newDirectory, "%s/Results/%s/run_%s", GPFpath, testname, this->timeOfTeststart);
    	this->makeDirectory(newDirectory);

		// make images subfolder
		sprintf(newSubdirectory,"%s/images", newDirectory);
    	this->makeDirectory(newSubdirectory);
		sprintf(this->imagestoragepath, "%s", newSubdirectory);

		// open file for image timetags
		sprintf(timetagStorageFileName, "%s/imageTimetags.csv", newSubdirectory);
		timetagStorageFile.open(timetagStorageFileName);

		// make subfolder for Guest Scientist data storage
		sprintf(newSubdirectory,"%s/GSdata", newDirectory);
    	this->makeDirectory(newSubdirectory);
		sprintf(this->GSstoragepath, "%s", newSubdirectory);
}

void dockingport_DataStorage::saveTimestampedImages(double imagetimestamp) {

	printf("Dockingport::saveTimestampedImages entered");

	// write time stamp together with current image number in file
	timetagStorageFile << numsaved << "," << imagetimestamp << "\n";

	// store images in Results folder
//	sprintf(leftImageStorageName, "%s/Left%d.bmp", this->imagestoragepath, numsaved);
//	sprintf(rightImageStorageName, "%s/Right%d.bmp", this->imagestoragepath, numsaved);
//	cv::imwrite(leftImageStorageName, leftframe);
//	cv::imwrite(rightImageStorageName, rightframe);

	sprintf(this->singleImageStorageName, "%s/SingleImage%d.bmp", this->imagestoragepath, numsaved);
	cv::imwrite(this->singleImageStorageName, this->storageImg);

	if(this->unrectifiedImageStorage) {
		sprintf(this->singleImageStorageNameUnrect, "%s/SingleImageUnrect%d.bmp", this->imagestoragepath, numsaved);
		cv::imwrite(this->singleImageStorageNameUnrect, this->storageImgUnrect);
	}

	numsaved++;

}

void dockingport_DataStorage::shutdownDataStorage() {

	timetagStorageFile.close();

}

int dockingport_DataStorage::getNumSaved() {
	return numsaved;
}

string dockingport_DataStorage::getGSdatastoragePath() {

	return string( this->GSstoragepath) + "/";
}

string dockingport_DataStorage::getGPFpath() {

	return string( this->GPFpath) + "/";
}


string dockingport_DataStorage::getTimeOfTestStart() {
	return string(this->timeOfTeststart);
}


string dockingport_DataStorage::newGSDataDirectory(char newDirName[200]) {
	char newDir[200];
    DIR *dir;

	sprintf(newDir, "%s/%s",this->GSstoragepath, newDirName);
    dir = opendir (newDir);
    if (dir == NULL)
    {
		// make new RESULTS directory for whole Program = Group
    	this->makeDirectory(newDir);
    	printf("New GSdata Directory Created: %s\n\r", newDir);
    }
	return string(newDir) + "/";
}

