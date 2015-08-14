#pragma once
#include <boost/thread.hpp>
#include <tobii/sdk/cpp/EyeTrackerBrowser.hpp>
#include <tobii/sdk/cpp/EyeTrackerBrowserFactory.hpp>
#include <tobii/sdk/cpp/EyeTrackerInfo.hpp>
#include <tobii/sdk/cpp/EyeTracker.hpp>
#include <tobii/sdk/cpp/Types.hpp>
#include <tobii/sdk/cpp/UpgradePackage.hpp>
#include <tobii/sdk/cpp/SyncManager.hpp>
#include <tobii/sdk/cpp/Library.hpp>
#include <tobii/sdk/cpp/MainLoop.hpp>
#include "../ofxTobiiAnalytics/MainLoopRunner.h"
#include "../ofxTobiiAnalytics/GazeEvent.h"

BEGIN_OFX_TOBII_ANALYTIKS_NAMESPACE

using namespace tobii::sdk::cpp;

class Tracker {
	
	bool tracking_;
	bool tracker_found_;
	string tracker_id_;
	
	MainLoopRunner *runner_;
	
	EyeTracker::pointer_t tracker;
	
public:
	
	bool isDeviceFound() const {
		return tracker_found_;
	}
	
	bool isTracking() const {
		return tracking_;
	}
	
	Tracker()
	: tracker_found_(false)
	, tracker_id_("")
	, tracking_(false)
	{
		tetio::Library::init();
	}
	
	void startTracking() {
		if (!isDeviceFound()) {
			ofLogError("device not found.");
			return;
		}
		
		tracking_ = true;
		tracker->startTracking();
		tracker->addGazeDataReceivedListener(boost::bind(&Tracker::onGazeDataReceived, this, _1));
	}
	
	void stopTracking() {
		if (!isDeviceFound()) {
			ofLogError("device not found.");
			return;
		}
		
		tracking_ = false;
		tracker->stopTracking();
	}
	
	void addCalibrationPoint(Point2d point) {
		tracker->addCalibrationPoint(point);
	}
	
	void startCalibration() {
		tracker->startCalibration();
	}
	
	void stopCalibration() {
		tracker->computeCalibration();
		tracker->stopCalibration();
	}
	
	void listTrackers() {
		
		runner_->start();
		
		EyeTrackerBrowser::pointer_t browser(EyeTrackerBrowserFactory::createBrowser(runner_->getMainLoop()));
		browser->addEventListener(boost::bind(&Tracker::onEyeTrackerBrowserEventList, this, _1, _2));
		
		startEyeTrackerLookUp(browser, "Browsing for eye trackers, please wait ...");
	}
	
	void printTrackerInfo(string tracker_id) {
		
		runner_->start();
		
		EyeTrackerBrowser::pointer_t browser(EyeTrackerBrowserFactory::createBrowser(runner_->getMainLoop()));
		browser->addEventListener(boost::bind(&Tracker::onEyeTrackerBrowserEventPrintInfo, this, _1, _2));
		
		startEyeTrackerLookUp(browser, "Browsing for " + tracker_id + ", please wait ...");
	}
	
	void open(string tracker_id)
	{
		/*
		  browsing for available eye trackers,
		  connecting to an eye tracker,
		  (optionally) configuring the eye tracker (calibration plane, frame rate, etc.),
		  (optionally) calibrating the eye tracker,
		  (optionally) synchronizing clocks with the eye tracker,
		  collecting eye gaze data and possibly acting on it in real time, e.g., for gaze-contingency studies.
		 */
		
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
			cout << " " << e.what() << " " << e.getErrorCode() << endl;
		}
	}
	
private:
	
	void startEyeTrackerLookUp(EyeTrackerBrowser::pointer_t browser,
							   string message) {
		
		cout << message << endl;
		
		browser->start();
		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
		browser->stop(); //!!! this is a blocking operation
		
		cout << "Browsing done." << endl;
	}
	
	void dumpTrackerInfo(EyeTrackerInfo::pointer_t info) {
		
		stringstream report;
		report << "ProductId: " << info->getProductId() << endl;
		report << "Status: " << info->getStatus() << endl;
		report << "Generation: " << info->getGeneration() << endl;
		report << "Model: " << info->getModel() << endl;
		report << "GivenName: " << info->getGivenName() << endl;
		report << "Version: " << info->getVersion() << endl;
		
		cout << report.str() << endl;
	}
	
#pragma mark -
#pragma mark events
	
	void onGazeDataReceived(tetio::GazeDataItem::pointer_t data) {
		
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
	
	void onEyeTrackerBrowserEventPrintInfo(EyeTrackerBrowser::event_type_t type,
										   EyeTrackerInfo::pointer_t info) {
		
		if (type == EyeTrackerBrowser::TRACKER_FOUND && info->getProductId() == tracker_id_)
		{
			tracker_found_ = true;
			dumpTrackerInfo(info);
		}
	}
	
	void onEyeTrackerBrowserEventList(tetio::EyeTrackerBrowser::event_type_t type,
									  tetio::EyeTrackerInfo::pointer_t info) {
		
		if (type == EyeTrackerBrowser::TRACKER_FOUND) {
			dumpTrackerInfo(info);
		}
		else {
			ofLogError() << "error: tracker was not found";
		}
	}
};

END_OFX_TOBII_ANALYTIKS_NAMESPACE