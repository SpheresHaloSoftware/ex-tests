#include "testproject.h"


testproject::testproject () {

	pthread_mutex_init(&keymutex, NULL);
}

void testproject::GSrunMain()
{
	this->captureAndRectifyImages(this->leftImage, this->rightImage);

	this->GSprocessImages(this->leftImage, this->rightImage);

}

void testproject::GSsetup(){

	opticsmountGSP::init();

	phase1.GSsetup(this);

	string imu_log_filename = this->datastorage.getGSdatastoragePath() + "imu_data.txt";
	imu_log_file.open(imu_log_filename.c_str());

	string forceTorque_log_filename = this->datastorage.getGSdatastoragePath() + "forceTorque_data.txt";
	forceTorque_log_file.open(forceTorque_log_filename.c_str());

	string globMet_log_filename = this->datastorage.getGSdatastoragePath() + "globMet_data.txt";
	globMet_log_file.open(globMet_log_filename.c_str());

	this->initVideoStreaming();
	this->captureAndRectifyImages(this->leftImage, this->rightImage);
}

void testproject::GScleanup() {
	imu_log_file.close();
	forceTorque_log_file.close();
	globMet_log_file.close();
	phase1.GScleanup();
	opticsmountGSP::shutdown();
}

void testproject::GSprocessImages(Mat& leftImage, Mat& rightImage) {
	phase1.GSprocessImages(leftImage, rightImage);

	pthread_mutex_lock(&keymutex);
	if (charKey == 0x1B) { // ESC
		printf("Quitting...\n");
		shutdownCriterion = true;
	}
	charKey = 0x00;
	pthread_mutex_unlock(&keymutex);
}

void testproject::GSprocessGuiKeyPress(unsigned char networkkey) {
	pthread_mutex_lock(&keymutex);
	charKey = networkkey;
	pthread_mutex_unlock(&keymutex);
}

void testproject::GSparseParameterFile(string line) {

	opticsmountGSP::parseParameterFile(line);

	phase1.GSparseParameterFile(line);
}

void testproject::GSparseCommandlineArgs(int argc, char *argv[]) {

// Read in path from environment variable
	if (argc>3) {

		sprintf(this->calibParamSetName, "%s", argv[3]);

	}
	else {

		cout << endl << endl << "Not enough command line arguments" << endl << "Please specify path to Camera Calibration Parameters!" << endl;

		cout << "Usage: <name of camera calibration set>" << endl << endl;

		exit(1);

	}


}

void testproject::GSprocessIMU(double timestamp, msg_imu_data imu_data) {
	imu_log_file << timestamp << "," << imu_data.testTime << ","  << imu_data.gyro[0] << "," << imu_data.gyro[1] << "," << imu_data.gyro[2] << "," << imu_data.accel[0] << "," << imu_data.accel[1] << "," << imu_data.accel[2] << std::endl;
}

void testproject::GSprocessForceTorque(double timestamp, msg_body_forces_torques FT_data) {
	forceTorque_log_file << timestamp << "," << FT_data.testTime << ","  << FT_data.forces[0] << "," << FT_data.forces[1] << "," << FT_data.forces[2] << "," << FT_data.torques[0] << "," << FT_data.torques[1] << "," << FT_data.torques[2] << std::endl;
}

void testproject::GSprocessGlobalMetrology(double timestamp, msg_state gm_data) {
	globMet_log_file << timestamp << "," << gm_data.testTime << ","
											<< gm_data.pos[0] << "," << gm_data.pos[1] << "," << gm_data.pos[2] << ","
											<< gm_data.vel[0] << "," << gm_data.vel[1] << "," << gm_data.vel[2] << ","
											<< gm_data.quat[0] << "," << gm_data.quat[1] << "," << gm_data.quat[2] << "," << gm_data.quat[3] << ","
											<< gm_data.angvel[0] << "," << gm_data.angvel[1] << "," << gm_data.angvel[2] << std::endl;
}

