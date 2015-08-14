#include "../ofxTobiiAnalytics/Common.h"
#include "../ofxTobiiAnalytics/Tracker.h"
#include "ofLog.h"

BEGIN_OFX_TOBII_ANALYTIKS_NAMESPACE

Tracker::Tracker()
: tracker_found_(false)
, tracker_id_("")
, tracking_(false)
{
	tetio::Library::init();
}

void Tracker::open(string tracker_id)
{
	uint32_t tetserverPort = 0;
	uint32_t syncPort = 0;
	
	try
	{
		EyeTrackerFactory::pointer_t eyeTrackerFactory = EyeTrackerBrowserFactory::createEyeTrackerFactoryByIpAddressOrHostname(tracker_id, tetserverPort, syncPort);
		
		if (eyeTrackerFactory) {
			cout << "Connecting ..." << endl;
			
			runner_ = new MainLoopRunner();
			runner_->start();
			tracker = eyeTrackerFactory->createEyeTracker(runner_->getMainLoop());
			
			tracker_id_ = tracker_id;
			tracker_found_ = true;
			cout << "Connected to: " << tracker_id_ << endl;
		}
		else {
			cerr << "The specified eyetracker could not be found." << endl;
		}
	}
	catch (EyeTrackerException e)
	{
		tracker_id_.clear();
		tracker_found_ = false;
		cout << " " << e.what() << " " << e.getErrorCode() << endl;
	}
}

void Tracker::startTracking() {
	if (!isDeviceFound()) {
		ofLogError("device not found.");
		return;
	}
	
	tracking_ = true;
	tracker->startTracking();
	tracker->addGazeDataReceivedListener(boost::bind(&Tracker::onGazeDataReceived, this, _1));
}

void Tracker::stopTracking() {
	if (!isDeviceFound()) {
		ofLogError("device not found.");
		return;
	}
	
	tracking_ = false;
	tracker->stopTracking();
}


#pragma mark -


bool Tracker::isDeviceFound() const {
	return tracker_found_;
}

bool Tracker::isTracking() const {
	return tracking_;
}


#pragma mark -


void Tracker::addCalibrationPoint(Point2d point) {
	tracker->addCalibrationPoint(point);
}

void Tracker::startCalibration() {
	tracker->startCalibration();
}

void Tracker::stopCalibration() {
	tracker->computeCalibration();
	tracker->stopCalibration();
}


#pragma mark -


void Tracker::listTrackers() {
	
	runner_->start();
	
	EyeTrackerBrowser::pointer_t browser(EyeTrackerBrowserFactory::createBrowser(runner_->getMainLoop()));
	browser->addEventListener(boost::bind(&Tracker::onEyeTrackerBrowserEventList, this, _1, _2));
	
	startEyeTrackerLookUp(browser, "Browsing for eye trackers, please wait ...");
}

void Tracker::printTrackerInfo(string tracker_id) {
	
	runner_->start();
	
	EyeTrackerBrowser::pointer_t browser(EyeTrackerBrowserFactory::createBrowser(runner_->getMainLoop()));
	browser->addEventListener(boost::bind(&Tracker::onEyeTrackerBrowserEventPrintInfo, this, _1, _2));
	
	startEyeTrackerLookUp(browser, "Browsing for " + tracker_id + ", please wait ...");
}


#pragma mark - Private


void Tracker::dumpTrackerInfo(EyeTrackerInfo::pointer_t info) {
	
	stringstream report;
	report << "ProductId: " << info->getProductId() << endl;
	report << "Status: " << info->getStatus() << endl;
	report << "Generation: " << info->getGeneration() << endl;
	report << "Model: " << info->getModel() << endl;
	report << "GivenName: " << info->getGivenName() << endl;
	report << "Version: " << info->getVersion() << endl;
	
	cout << report.str() << endl;
}

void Tracker::startEyeTrackerLookUp(EyeTrackerBrowser::pointer_t browser,
						   string message) {
	
	cout << message << endl;
	
	browser->start();
	boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	browser->stop(); //!!! this is a blocking operation
	
	cout << "Browsing done." << endl;
}

void Tracker::onGazeDataReceived(tetio::GazeDataItem::pointer_t data) {
	
	static GazeEvent e;
	
	e.timestamp = data->timestamp;
	
	e.leftEyePosition3d = ofVec3f(data->leftEyePosition3d.x,
								  data->leftEyePosition3d.y,
								  data->leftEyePosition3d.z);
	
	e.rightEyePosition3d = ofVec3f(data->rightEyePosition3d.x,
								   data->rightEyePosition3d.y,
								   data->rightEyePosition3d.z);
	
	e.leftValidity = data->leftValidity;
	
	e.rightValidity = data->rightValidity;
	
	e.leftEyePosition3dRelative = ofVec3f(data->leftEyePosition3dRelative.x,
										  data->leftEyePosition3dRelative.y,
										  data->leftEyePosition3dRelative.z);
	
	e.rightEyePosition3dRelative = ofVec3f(data->rightEyePosition3dRelative.x,
										   data->rightEyePosition3dRelative.y,
										   data->rightEyePosition3dRelative.z);
	
	e.leftGazePoint3d = ofVec3f(data->leftGazePoint3d.x,
								data->leftGazePoint3d.y,
								data->leftGazePoint3d.z);
	
	e.rightGazePoint3d = ofVec3f(data->rightGazePoint3d.x,
								 data->rightGazePoint3d.y,
								 data->rightGazePoint3d.z);
	
	e.leftGazePoint2d = ofVec2f(data->leftGazePoint2d.x,
								data->leftGazePoint2d.y);
	
	e.rightGazePoint2d = ofVec2f(data->rightGazePoint2d.x,
								 data->rightGazePoint2d.y);;
	
	e.leftPupilDiameter = data->leftPupilDiameter;
	
	e.rightPupilDiameter = data->rightPupilDiameter;
	
	ofNotifyEvent(GazeEvent::events, e);
}

void Tracker::onEyeTrackerBrowserEventPrintInfo(EyeTrackerBrowser::event_type_t type,
									   EyeTrackerInfo::pointer_t info) {
	
	if (type == EyeTrackerBrowser::TRACKER_FOUND && info->getProductId() == tracker_id_)
	{
		tracker_found_ = true;
		dumpTrackerInfo(info);
	}
}

void Tracker::onEyeTrackerBrowserEventList(tetio::EyeTrackerBrowser::event_type_t type,
								  tetio::EyeTrackerInfo::pointer_t info) {
	
	if (type == EyeTrackerBrowser::TRACKER_FOUND) {
		dumpTrackerInfo(info);
	}
	else {
		ofLogError() << "error: tracker was not found";
	}
}

END_OFX_TOBII_ANALYTIKS_NAMESPACE