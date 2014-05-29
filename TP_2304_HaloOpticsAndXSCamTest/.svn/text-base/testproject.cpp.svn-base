#include "testproject.h"

testproject::testproject () {
	maneuverNumber = 1;
	averageTime = 0;
	time_samples = 0;
	max_depth = 100.0;
	min_depth = 0.01;
	SubSample = false;
	equalize = false;
	pthread_mutex_init(&keymutex, NULL);
}

void testproject::GSsetup(){

	opticsmountGSP::init();

	dockingportGSP::init();


	videostreaming.createNew_MatVideoBuffer(doubleVideoBuffer,"OM Left DP Right");
	videostreaming.setAsDefaultVideoMode(doubleVideoBuffer);

	save_frame_number = 0;

	initVideoStreaming();
	// Take one image for each to remove a possibly bad first image before GSrunMain
	this->captureAndRectifySingleImage(this->singleImage);
	this->captureAndRectifyImages(this->leftImage,this->rightImage);

}

void testproject::GSrunMain()
{

	this->captureAndRectifySingleImage(this->singleImage);
	this->captureAndRectifyImages(this->leftImage,this->rightImage);
	this->GSstreamImages(this->leftImage,this->rightImage,this->singleImage);

}

void testproject::GSstreamImages(cv::Mat& leftImg,cv::Mat& rightImg,cv::Mat& singleImg){
	Mat doubleImg;

	Size size( leftImage.cols + singleImg.cols, MAX(leftImage.rows, singleImage.rows) );

	//place two images side by side
	doubleImg.create( size, CV_MAKETYPE(leftImage.depth(), 3) );
	Mat doubleImgLeft = doubleImg( Rect(0, 0, leftImage.cols, leftImage.rows) );
	Mat doubleImgRight = doubleImg( Rect(leftImage.cols, 0, singleImage.cols, singleImage.rows) );

	cvtColor( leftImage, doubleImgLeft, CV_GRAY2BGR );
	singleImage.copyTo(doubleImgRight);

	if (opticsmountGSP::datastorage.autoImageStorage) {
			stringstream filename;
			filename << opticsmountGSP::datastorage.getGSdatastoragePath() << "SideBySideImg" << save_frame_number << ".bmp";
			cv::imwrite(filename.str(), doubleImg);
			filename.clear();


			filename << opticsmountGSP::datastorage.getGSdatastoragePath() << "SingleImg" << save_frame_number << ".bmp";
			cv::imwrite(filename.str(), singleImage);
			filename.clear();


			filename << opticsmountGSP::datastorage.getGSdatastoragePath() << "RightImg" << save_frame_number << ".bmp";
			cv::imwrite(filename.str(), rightImage);
			filename.clear();

			save_frame_number++;
	}


	if (videostreaming.videoStreamingOn) {
		videostreaming.update_MatVideoBuffer(doubleVideoBuffer, doubleImg);
	}


}

void testproject::GSbackgroundTask() {

}

void testproject::GSparseParameterFile(string line) {
	  string searchString;
	  string foundString;
	  size_t found;
	  
	  dockingportGSP::parseParameterFile(line);
	  opticsmountGSP::parseParameterFile(line);

	  searchString = "SUBSAMPLE";
	  found = line.find(searchString);
	  if (found != string::npos)
	  {
		foundString = line.substr( found+searchString.size()+1, string::npos );
		if (foundString == "true") {
			this->SubSample = true;
		}
		cout << "SUBSAMPLE:  " << foundString << endl;
	  }
	  searchString.clear();
	  found = string::npos;

	  searchString = "EQUALIZE";
	  found = line.find(searchString);
	  if (found != string::npos)
	  {
		foundString = line.substr( found+searchString.size()+1, string::npos );
		if (foundString == "true") {
			this->equalize = true;
		}
		cout << "EQUALIZE:  " << foundString << endl;
	  }
	  searchString.clear();
	  found = string::npos;

	  searchString = "MIN_DEPTH";
	  found = line.find(searchString);
	  if (found != string::npos)
	  {
		  foundString = line.substr( found+searchString.size()+1, string::npos );
		  min_depth =  atof(foundString.c_str());
		  cout << "MIN_DEPTH: " << min_depth << endl;
	  }
	  searchString.clear();
	  found = string::npos;

	  searchString = "MAX_DEPTH";
	  found = line.find(searchString);
	  if (found != string::npos)
	  {
		  foundString = line.substr( found+searchString.size()+1, string::npos );
		  max_depth =  atof(foundString.c_str());
		  cout << "MAX_DEPTH: " << max_depth << endl;
	  }
	  searchString.clear();
	  found = string::npos;
}

void testproject::GScleanup() {
	
	dockingportGSP::shutdown();
	opticsmountGSP::shutdown();

}

void testproject::GSparseCommandlineArgs(int argc, char *argv[]) {

// Read in path from environment variable
	if (argc>3) {

		sprintf(opticsmountGSP::calibParamSetName, "%s", argv[3]);
		sprintf(dockingportGSP::calibParamSetName, "%s", argv[3]);

	}
	else {

		cout << endl << endl << "Not enough command line arguments" << endl << "Please specify path to Camera Calibration Parameters!" << endl;

		cout << "Usage: <name of camera calibration set>" << endl << endl;

		exit(1);

	}


}


