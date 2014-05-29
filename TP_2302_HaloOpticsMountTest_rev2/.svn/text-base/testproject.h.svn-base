#ifndef _TESTPROJECT_H_
#define _TESTPROJECT_H_

#include "haloGSP.h"
#include "../../OpticsMountCore/opticsmountGSP.h"
#include "phase1.h"

class testproject : public opticsmountGSP {

	Phase1 phase1;

	char key;
	pthread_mutex_t keymutex;
	unsigned char charKey;

	ofstream imu_log_file, forceTorque_log_file, globMet_log_file;


public:

	testproject ();
	void GSrunMain();
	void GSsetup();
	void GSprocessImages(Mat& leftImage, Mat& rightImage);
	void GSprocessGuiKeyPress(unsigned char networkkey);
	void GSparseParameterFile(string line);
	void GSparseCommandlineArgs(int argc, char *argv[]);
	void GSprocessIMU(double timestamp, msg_imu_data imu_data);
	void GSprocessForceTorque(double timestamp, msg_body_forces_torques FT_data);
	void GSprocessGlobalMetrology(double timestamp, msg_state gm_data);
	void GScleanup();

};

#endif
