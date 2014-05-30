#include "haloGSP.h"

string HaloGSP::execAndReturnSystemOutput(char* cmd) {

	// executes system command with popen, which writes to a file and "system" output is read from this file

    FILE* fileWhereReturnValIsStored = popen(cmd, "r");
    if (!fileWhereReturnValIsStored)
    	return "ERROR";
    char buffer[128];
    string result = "";

    while(!feof(fileWhereReturnValIsStored)) {
        if(fgets(buffer, 128, fileWhereReturnValIsStored) != NULL)
                result += buffer;
    }

    pclose(fileWhereReturnValIsStored);
    return result;
}

void HaloGSP::parseCommandlineArgs(int argc, char *argv[]) {
// argv[0]	by default: path and executable name
// argv[1]	Full HPF directory
// argv[2]	ParameterFile name
// argv[3]	Customizable by Guest Scientist 
// ...		Customizable by Guest Scientist 
// argv[argc]	Customizable by Guest Scientist 

	// Extract Executable name from argv[0]
	// testname = Executable name
	string searchString = argv[0];
	string foundString;
	int lastSlashPos;

	size_t strLength = searchString.length();
	int strLengthInt = int(strLength);
	int k;
	for ( k=(strLengthInt-1); k>=0; k--)
	{
	  if (searchString[k] == '/')
	  {
		  lastSlashPos = k;
		  break;
	  }

	}
	foundString = searchString.substr( lastSlashPos+1, string::npos );
	strcpy(this->testname, foundString.c_str());
	cout << "testname " << foundString << endl;


	// Read in path from environment variable
	if (argc>2) {

		sprintf(this->HPFpath, "%s", argv[1]);

		sprintf(this->parameterFileName, "%s", argv[2]);

	}
	else {

		cout << endl << endl << "Not enough command line arguments" << endl << "Please specify path to HPF and parameterFileName!" << endl;

		cout << "Usage: ./bin/<testExecutableName> <HPF directory path> <name of parameterFile>" << endl << endl;

		exit(1);

	}
	
	if (argc>3) {

		GSparseCommandlineArgs(argc,argv);

	}

}

void HaloGSP::parseParameterFile() {

	  string line;
	  string searchString;
	  string foundString;
	  size_t found;

	  char parameterFileDir[200];
	  sprintf(this->parameterFileDir, "%s/ParameterFiles/%s", this->HPFpath, this->testname);
	  sprintf(this->parameterFilePath, "%s/%s", this->parameterFileDir, this->parameterFileName);
	  ifstream parameterFile(parameterFilePath);

	  if (parameterFile.is_open())
	  {
		  cout << endl << endl << "--- PARAMETERFILE INPUT ---" << endl;

	    while ( parameterFile.good() )
	    {
	      getline (parameterFile, line);

	      if (line[0] != '#')
	      {
			  
			  // useSpheres
			  searchString = "USE_SPHERES";
			  found = line.find(searchString);
			  if (found != string::npos)
			  {
				  foundString = line.substr( found+searchString.size()+1, string::npos );
				  if (foundString == "true")
					  this->spheres.useSpheres = true;
				  if (foundString == "false")
					  this->spheres.useSpheres = false;
				  cout << "USE_SPHERES " << foundString << endl;
			  }
			  searchString.clear();
			  found = string::npos;

			  // videoStreamingOn
			  	  searchString = "VIDEO_STREAMING_ON";
			  	  found = line.find(searchString);
			  	  if (found != string::npos)
			  	  {
			  		  foundString = line.substr( found+searchString.size()+1, string::npos );
			  		  if (foundString == "true")
			  			  this->videostreaming.videoStreamingOn = true;
			  		  if (foundString == "false")
			  			  this->videostreaming.videoStreamingOn = false;
			  		  cout << "VIDEO_STREAMING_ON " << foundString << endl;
			  	  }
			  	  searchString.clear();
			  	  found = string::npos;

			  this->GSparseParameterFile(line);

	      }

	    }
	    parameterFile.close();
		cout << "---------------------------" << endl;

	  }

	  else
		 cout << "Unable to open parameterFile.txt !" << endl;

	  //other files on the filesystem

	  // Open the /opt/GogglesDaemon/TEST_PROJ_CHANNEL file
	  ifstream channelFile(TEST_PROJ_CHANNEL_FILE);

	  if (channelFile.is_open())
	  {
		  string ipaddr, port_str;
		  int port;

		  getline(channelFile, line);
		  cout << "Comm Mode: " << line << endl;

		  if (line == "WifiCom" || line == "EthCom") {
			  getline(channelFile, line);
			  //ipaddr_port = line.substr( found+searchString.size()+1, string::npos );
			  found = line.find(":");
			  if (found != string::npos) {
				  ipaddr = line.substr(0,found);
				  port_str = line.substr(found+1,string::npos);
				  port = atoi(port_str.c_str());
			  }
			  cout << "VIDEO SERVER IP ADDRESS:PORT: " << ipaddr << ":" << port << endl;
			  videostreaming.setIPAddrPort(ipaddr, port);
		  } else {
			  getline(channelFile, line); //keep in to get 2nd line that is gogglesDaemon serial port
			  videostreaming.videoStreamingOn = false;
			  cout << "VIDEO_STREAMING_DISABLED - NO IP CONNECTION" << endl;
		  }

		  //serial port
		  getline(channelFile, line);
		  this->spheres.serial_port_path = line;
		  cout << "Serial Com: " << this->spheres.serial_port_path << endl;

	  }
	  else {
		  cout << "Unable to open: " << TEST_PROJ_CHANNEL_FILE << endl;
	  }

}

///////////////// Threads

void HaloGSP::spheres_thread() {

	unsigned char msg[256];
	unsigned char messagelength;
	msg_header msgHeader;
	int retVal, totalBytes;

	printf("spheres_thread started\n");

	while(shutdownCriterion == false)
	{

		totalBytes = 0;
		while (totalBytes < sizeof(msgHeader)) {
			retVal = read(spheres.serial_fd, (unsigned char*)&msgHeader+totalBytes, sizeof(msgHeader)-totalBytes);
			if (retVal == -1)
			{
				printf("read() message failed: %s\n", strerror(errno));
				this->shutdownCriterion = true;
			}
			totalBytes+=retVal;
		}

		if (msgHeader.startByte != START_BYTE || msgHeader.from != FROM_SPHERES) {
			printf("Bad start byte\n");
			continue;
		}

		totalBytes = 0;

		while (totalBytes < msgHeader.length) {
			retVal = read(spheres.serial_fd, msg+totalBytes, msgHeader.length-totalBytes);
			if (retVal == -1)
			{
				printf("read() message failed: %s\n", strerror(errno));
				this->shutdownCriterion = true;
			}
			totalBytes+=retVal;
		}

		retVal = spheres.parseString(msgHeader, msg);
		if (retVal == -1) {
			this->shutdownCriterion = true;
		}

	}

	pthread_exit(0);

}
void HaloGSP::videoStreaming_thread() {

	int requestedMode = 0;

	videostreaming.sendVideoModes();

	while(shutdownCriterion == false)
	{
		videostreaming.receiveMessages();

	//	pthread_mutex_lock(&VideoStreamingMutex);
		videostreaming.sendVideoStream();
	//	pthread_mutex_unlock(&VideoStreamingMutex);

		usleep(10000);
	}
	pthread_exit(0);
}

/*
void HaloGSP::dataStorage_thread() {

	while(!stopDataStorageThread)
	{
		// Add code here for data storage
	}
	pthread_exit(0);
}
*/
void HaloGSP::imuProcessing_thread() {

	while(shutdownCriterion == false)
	{
		if (!this->spheres.imuData_dq.empty()) {
			struct timespec timeA, timeB;
			clock_gettime(CLOCK_REALTIME, &timeA);

			pthread_mutex_lock(&this->spheres.imuProcessingMutex);
			timestampedIMU& timu = this->spheres.imuData_dq.back();
			pthread_mutex_unlock(&this->spheres.imuProcessingMutex);

			this->GSprocessIMU(timu.getTimestamp(), timu.getIMU());

			pthread_mutex_lock(&this->spheres.imuProcessingMutex);
			this->spheres.imuData_dq.pop_back();
			pthread_mutex_unlock(&this->spheres.imuProcessingMutex);

			clock_gettime(CLOCK_REALTIME, &timeB);
			double dt = timeDiff(&timeA, &timeB);

			int deque_size = this->spheres.imuData_dq.size();
			if ( deque_size > 1) {
				std::cout << "IMU Deque Size: " << deque_size << ". Timestamp: " << timu.getTimestamp() << "Write time: " << dt << std::endl;
			}
		} else {
			this->spheres.imuData_dq.clear();
			usleep(10000);
		}
	}
	pthread_exit(0);
}

void HaloGSP::forceTorqueProcessing_thread() {

	while(shutdownCriterion == false)
	{
		if (!this->spheres.forceTorqueData_dq.empty()) {
			struct timespec timeA, timeB;
			clock_gettime(CLOCK_REALTIME, &timeA);

			pthread_mutex_lock(&this->spheres.forceTorqueProcessingMutex);
			timestampedForceTorque& tft = this->spheres.forceTorqueData_dq.back();
			pthread_mutex_unlock(&this->spheres.forceTorqueProcessingMutex);

			this->GSprocessForceTorque(tft.getTimestamp(), tft.getForceTorque());

			pthread_mutex_lock(&this->spheres.forceTorqueProcessingMutex);
			this->spheres.forceTorqueData_dq.pop_back();
			pthread_mutex_unlock(&this->spheres.forceTorqueProcessingMutex);

			clock_gettime(CLOCK_REALTIME, &timeB);
			double dt = timeDiff(&timeA, &timeB);

			int deque_size = this->spheres.forceTorqueData_dq.size();
			if ( deque_size > 1) {
				std::cout << "FT Deque Size: " << deque_size << ". Timestamp: " << tft.getTimestamp() << "Write time: " << dt << std::endl;
			}
		} else {
			this->spheres.forceTorqueData_dq.clear();
			usleep(10000);
		}
	}
	pthread_exit(0);
}

void HaloGSP::globMetProcessing_thread() {

	while(shutdownCriterion == false)
	{
		if (!this->spheres.globMetData_dq.empty()) {
			struct timespec timeA, timeB;
			clock_gettime(CLOCK_REALTIME, &timeA);

			pthread_mutex_lock(&this->spheres.globMetProcessingMutex);
			timestampedState& tstate = this->spheres.globMetData_dq.back();
			pthread_mutex_unlock(&this->spheres.globMetProcessingMutex);

			this->GSprocessGlobalMetrology(tstate.getTimestamp(), tstate.getState());

			pthread_mutex_lock(&this->spheres.globMetProcessingMutex);
			this->spheres.globMetData_dq.pop_back();
			pthread_mutex_unlock(&this->spheres.globMetProcessingMutex);

			clock_gettime(CLOCK_REALTIME, &timeB);
			double dt = timeDiff(&timeA, &timeB);

			int deque_size = this->spheres.globMetData_dq.size();
			if ( deque_size > 1) {
				std::cout << "GlobMet Deque Size: " << deque_size << ". Timestamp: " << tstate.getTimestamp() << "Write time: " << dt << std::endl;
			}
		} else {
			this->spheres.globMetData_dq.clear();
			usleep(10000);
		}
	}

	pthread_exit(0);
}


void HaloGSP::initBackgroundTask() {

		int retVal = pthread_create(&BackgroundTaskThread, NULL, this->backgroundTask_thread_Helper, this);
		if (retVal != 0)
		{
			printf("pthread_create BackgroundTaskThread failed\n");
			return exit(1);
		}
}


void HaloGSP::backgroundTask_thread() {

	while(shutdownCriterion == false)
	{
		this->GSbackgroundTask();
		usleep(10000);
	}
	pthread_exit(0);
}


void HaloGSP::powerOnPort(int port_number){
	// Add code here
}


void HaloGSP::powerOffPort(int port_number){
	// Add code here
}

/*void HaloGSP::sendDataToPort(int port_number, GSdataStructure data){
	// Add code here
}

GSdataStructure HaloGSP::getDataFromPort(int port_number){
	// Add code here
}
*/
void HaloGSP::init()
{

/////////////////// Read from ParameterFile

	this->parseParameterFile();

///////////////////


///////////////////	Spheres Initialization
	if (spheres.useSpheres) {

		GSPretVal = spheres.initSpheres();
		if (GSPretVal != 0)
		{
			printf("Initializing Spheres failed\n");
			return exit(1);
		}

		// create spheres thread
		GSPretVal = pthread_create(&SpheresThread, NULL, this->spheres_thread_Helper, this);
		if (GSPretVal != 0)
		{
			printf("pthread_create SpheresThread failed\n");
			return exit(1);
		}
	}

///////////////////

	//IMU Processing Thread
	// create thread
	GSPretVal = pthread_create(&ImuProcessingThread, NULL, this->imuProcessing_thread_Helper, this);
	if (GSPretVal != 0)
	{
		printf("pthread_create ImuProcessingThread failed\n");
		return exit(1);
	}

	//Force and Torque Processing Thread
	// create thread
	GSPretVal = pthread_create(&ForceTorqueProcessingThread, NULL, this->forceTorqueProcessing_thread_Helper, this);
	if (GSPretVal != 0)
	{
		printf("pthread_create ForceTorqueProcessingThread failed\n");
		return exit(1);
	}

	//Global Metrology Processing Thread
	// create thread
	GSPretVal = pthread_create(&GlobMetProcessingThread, NULL, this->globMetProcessing_thread_Helper, this);
	if (GSPretVal != 0)
	{
		printf("pthread_create GlobMetProcessingThread failed\n");
		return exit(1);
	}


/////////////////// GS Customizable Threads
	this->GScustomThreads();
///////////////////

}

void HaloGSP::initVideoStreaming()
{

///////////////////  Init VideoStreaming and Start VideoStreaming Thread
	cout << "in initVideo" << endl;
	if (videostreaming.videoStreamingOn)
	{
		videostreaming.initVideoStreaming(this);
		cout << "in loop after init streaming" << endl;
		// create VideoStreaming thread
		int retVal = pthread_create(&VideoStreamingThread, NULL, this->videoStreaming_thread_Helper, this);
		cout << "in loop after thread creation" << endl;
		if (retVal != 0)
		{
			printf("pthread_create VideoStreamingThread failed\n");
			return exit(1);
		}
	}

}

void HaloGSP::GScustomThreads(){
	// Creation of additional custom threads goes here
}


void HaloGSP::GSsetup(){
	// Guest Scientist setup procedures go here

}


void HaloGSP::GScleanup(){
	// Guest Scientist cleanup procedures go here

}


void HaloGSP::GSparseCommandlineArgs(int argc,char *argv[]){
	// Additional Guest Scientist command line argument parsing goes here
}

void HaloGSP::GSparseParameterFile(string line) {
	// Additional Guest Scientist parameter file parsing goes here
}


void HaloGSP::GSrunMain(){
	// Main loop for Guest Scientist actions, algorithms, and data processing
}


void HaloGSP::GSbackgroundTask(){
	// Guest Scientist background actions go here
}


void HaloGSP::GSprocessGuiMouseClick(int x, int y) {
	cout << "GSP: Mouse Click: [" << x << ", " << y << endl;
}


void HaloGSP::GSprocessGuiKeyPress(unsigned char key) {
	cout << "GSP: Key Press: " << key << endl;
}


void HaloGSP::GSprocessIMU(double timestamp, msg_imu_data imu_data) {

	cout << "Time: " << timestamp << ", ";
	cout << "Test Time: " << imu_data.testTime << ", ";
	cout << "Gyro, Accel: [" << imu_data.gyro[0] << "," << imu_data.gyro[1] << "," << imu_data.gyro[2] << "],";
	cout << "Accel: [" << imu_data.accel[0] << "," << imu_data.accel[1] << "," << imu_data.accel[2] << "]" << endl;

}


void HaloGSP::GSprocessForceTorque(double timestamp, msg_body_forces_torques ft_data) {

	cout << "Time: " << timestamp << ", ";
	cout << "Test Time: " << ft_data.testTime << ", ";
	cout << "Forces: [" << ft_data.forces[0] << "," << ft_data.forces[1] << "," << ft_data.forces[2] << "],";
	cout << "Torques: [" << ft_data.torques[0] << "," << ft_data.torques[1] << "," << ft_data.torques[2] << "]" << endl;

}


void HaloGSP::GSprocessGlobalMetrology(double timestamp, msg_state gm_data) {

	cout << "Time: " << timestamp << ", ";
	cout << "Test Time: " << gm_data.testTime << ", ";
	cout << "Pos: [" << gm_data.pos[0] << "," << gm_data.pos[1] << "," << gm_data.pos[2] << "],";
	cout << "Vel: [" << gm_data.vel[0] << "," << gm_data.vel[1] << "," << gm_data.vel[2] << "],";
	cout << "Quat: [" << gm_data.quat[0] << "," << gm_data.quat[1] << "," << gm_data.quat[2] << "," << gm_data.quat[3] << "],";
	cout << "Angvel: [" << gm_data.angvel[0] << "," << gm_data.angvel[1] << "," << gm_data.angvel[2] << "],";

}


void HaloGSP::shutdown() {

	if (spheres.useSpheres)
	{
		GSPretVal = spheres.closeSpheres();
		if (GSPretVal != 0)
		{
			printf("Closing Spheres failed \n");
		}

		sleep(2);	//wait for log data to finish processing
	}

	if (videostreaming.videoStreamingOn) {
		GSPretVal = videostreaming.close();
		if (GSPretVal != 0)
		{
			printf("Closing Video failed \n");
		}
	}
}


double HaloGSP::timeDiff(struct timespec *start, struct timespec *end) {
	double milliseconds;
	double tv_sec, tv_nsec;
	if ((end->tv_nsec-start->tv_nsec)<0) {
		tv_sec = end->tv_sec-start->tv_sec-1;
		tv_nsec = 1.0e9+end->tv_nsec-start->tv_nsec;
	} else {
		tv_sec = end->tv_sec-start->tv_sec;
		tv_nsec = end->tv_nsec-start->tv_nsec;
	}
	milliseconds = tv_sec*1.0e3 + (tv_nsec / 1.0e6);
	return milliseconds;
}


double HaloGSP::updateTime() {
	clock_gettime(CLOCK_REALTIME, &currentTime);
	testtime = timeDiff(&testStartTime, &currentTime);
	return testtime;
//	printf("Test time: %f\n", testtime);
}


void HaloGSP::runMain(int argc, char *argv[]) {

    std::time_t result = std::time(NULL);
    std::cout << std::asctime(std::localtime(&result))
              << result << " seconds since the Epoch\n";

	this->parseCommandlineArgs(argc, argv);

	this->init();

	this->GSsetup();

	if (this->useBackgroundTask) {

		this->initBackgroundTask();
	}

	if (spheres.useSpheres)
	{
		this->spheres.waitGspInitTest();
	}

	clock_gettime(CLOCK_REALTIME, &testStartTime);

//	zerotime = timeRAW.tv_sec*1000+timeRAW.tv_usec/1000; // Time when main program loop is entered in ms, used for image time tags


	if (spheres.useSpheres)
	{
		while(shutdownCriterion == false)
		{
			this->shutdownCriterion = this->spheres.checkTestTerminate();

			this->GSrunMain();
		}
	}

	else // one if less within the loop
	{
		while(shutdownCriterion == false)
		{
			this->GSrunMain();
		}
	}

	this->GScleanup();

	this->shutdown();

	cout << endl << "Test has been terminated." << endl << endl;

}


