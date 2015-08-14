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
	
	Tracker();
	void open(string tracker_id);
	void startTracking();
	void stopTracking();
	
	bool isDeviceFound() const;
	bool isTracking() const;
	
	void startCalibration();
	void addCalibrationPoint(Point2d point);
	void stopCalibration();
	
	void listTrackers();
	void printTrackerInfo(string tracker_id);
	
private:
	
	void dumpTrackerInfo(EyeTrackerInfo::pointer_t info);
	void startEyeTrackerLookUp(EyeTrackerBrowser::pointer_t browser, string message);
	
	void onGazeDataReceived(tetio::GazeDataItem::pointer_t data);
	void onEyeTrackerBrowserEventPrintInfo(EyeTrackerBrowser::event_type_t type, EyeTrackerInfo::pointer_t info);
	void onEyeTrackerBrowserEventList(tetio::EyeTrackerBrowser::event_type_t type, tetio::EyeTrackerInfo::pointer_t info);
};

END_OFX_TOBII_ANALYTIKS_NAMESPACE