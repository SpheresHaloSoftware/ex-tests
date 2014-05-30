/*!
 *  \brief     Guest Scientist Program for tests involving the Halo.
 *  \details   This is the primary GSP class for tests involving the Halo.
 *  \author    Bryan McCarthy
 *  \author    Chris Jewison
 *  \version   0.1
 *  \date      Feb 2014
 *  \copyright TBD
 */

#ifndef _HALO_GSP_H_
#define _HALO_GSP_H_

class HaloGSP;

// include all class headers here
#include "spheres.h"
#include "videoStreaming.h"
#include "networking.h"

// C++ standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <sys/resource.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <ctime>
#include <cerrno>
#include <deque>

using namespace std;

#define TEST_PROJ_CHANNEL_FILE "/opt/GogglesDaemon/TEST_PROJ_CHANNEL"

class HaloGSP
{


protected:


private:

	struct timespec testStartTime, currentTime;


	// Threads
	pthread_t SpheresThread;
	pthread_t DataStorageThread; // Modified from Image
	pthread_t ImuProcessingThread;
	pthread_t ForceTorqueProcessingThread;
	pthread_t GlobMetProcessingThread;
	pthread_t BackgroundTaskThread;
	pthread_t VideoStreamingThread;

	// Mutexes
	pthread_mutex_t storageDeque_mutex;
	pthread_mutex_t VideoStreamingMutex;
	//std::deque<timestampedImage> storageImg_deque;

public:

	//test time in millisecond
	double testtime;

	// GSP Flags
	bool shutdownCriterion; // while loop executed until this is true or test ended by SPHERES
	int GSPretVal;
	bool useBackgroundTask;

	// Variables to store paths
	char HPFpath[200]; // path to HPF folder, read in from command line
	char parameterFilePath[200];
	char parameterFileDir[200];
	char parameterFileName[100];

	// testname, specified in ParameterFile
	char testname[100];

	// Videostreaming
	string currentVideoStreamingModeName;
	string GSvideoStreamingModeFrameNames;


	VideoStreaming videostreaming;
	Spheres spheres;

	// declare default videoBuffer
	MatVideoBuffer defaultVideoBuffer;



	HaloGSP() {

		// internal flag init settings
		shutdownCriterion = false;

		pthread_mutex_init(&VideoStreamingMutex, NULL);

	}
	
	//// Core Functionalities
private:

	void init();

	void initBackgroundTask();

	void shutdown();

public:

	virtual void initVideoStreaming(); // called after GSsetup, since GS input needed first

	virtual void GScustomThreads();

	virtual void GSsetup();

	virtual void GSbackgroundTask(); //background processing running at 10ms loop (if enabled with useBackgroundTask)

	virtual void GSprocessIMU(double timestamp, msg_imu_data imu_data); // IMU data processing science algorithms

	virtual void GSprocessForceTorque(double timestamp, msg_body_forces_torques forceTorque_data); // Force and torque (commanded) data processing science algorithms

	virtual void GSprocessGlobalMetrology(double timestamp, msg_state globMet_data); // Global metrology (13 element state vector) data processing science algorithms

	virtual void GSrunMain(); // Main loop for Guest Scientist code

	virtual void GScleanup(); // potential cleanup function that can be called after test is finished to store/delete data

	virtual	void GSparseCommandlineArgs(int argc,char *argv[]);

	virtual void GSparseParameterFile(string line);

	virtual void GSprocessGuiMouseClick(int x, int y);		//process a mouseclick from the GUI

	virtual void GSprocessGuiKeyPress(unsigned char key);

	void runMain(int argc, char *argv[]);


	//// Supplementary Functionalities
public:

	double updateTime();

	// set variables for Videostreaming buffer

//Moved to here from the private section
	//updates the testtime variable
	double timeDiff(struct timespec *start, struct timespec *end);

	// starting a Linux process and returning "system" output
	string execAndReturnSystemOutput(char* cmd);
//End moved to here from the private section
private:



	// Parsing
	void parseCommandlineArgs(int argc, char *argv[]);

	void parseParameterFile();

	////// Threads
    // Spheres Thread
	void spheres_thread();

    static void * spheres_thread_Helper(void * This)
    {
    	((HaloGSP *)This)->spheres_thread();
    	return NULL;
    }

    // Video Streaming Thread
    	void videoStreaming_thread();

        static void * videoStreaming_thread_Helper(void * This)
        {
        	((HaloGSP *)This)->videoStreaming_thread();
        	return NULL;
        }

	// Data Storage Thread
    void dataStorage_thread();

    static void * dataStorage_thread_Helper(void * This)
    {
    	((HaloGSP *)This)->dataStorage_thread();
    	return NULL;
    }

    // IMU Processing Thread
	void imuProcessing_thread();

    static void * imuProcessing_thread_Helper(void * This)
    {
    	((HaloGSP *)This)->imuProcessing_thread();
    	return NULL;
    }

    // Forces Torques Processing Thread
	void forceTorqueProcessing_thread();

    static void * forceTorqueProcessing_thread_Helper(void * This)
    {
    	((HaloGSP *)This)->forceTorqueProcessing_thread();
    	return NULL;
    }

    // Forces Torques Processing Thread
	void globMetProcessing_thread();

    static void * globMetProcessing_thread_Helper(void * This)
    {
    	((HaloGSP *)This)->globMetProcessing_thread();
    	return NULL;
    }

    // Background Task Thread
	void backgroundTask_thread();

    static void * backgroundTask_thread_Helper(void * This)
    {
    	((HaloGSP *)This)->backgroundTask_thread();
    	return NULL;
    }

    // Function to power on certain port
    void powerOnPort(int port_number);

    // Function to power off certain port
    void powerOffPort(int port_number);
/*
    // Function to send data to a certain port
    void sendDataToPort(int port_number, GSdataStructure data);
	
	// Function to get data from a certain port
	GSdataStructure getDataFromPort(int port_number);
*/
public:

    // Signal Handling, if Program shall be terminated (Either by IP Comm Block or CTRL+C)
    // This Function is not a class member on purpose
    void terminateHandler(int sig)
    {
    	cout << endl << endl << "Program terminated with Signal: " << sig << endl;
    	this->shutdownCriterion = true;

    }

};


#endif
