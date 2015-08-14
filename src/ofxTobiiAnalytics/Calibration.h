#pragma once

#include <tobii/sdk/cpp/Types.hpp>

BEGIN_OFX_TOBII_ANALYTIKS_NAMESPACE

using namespace tobii::sdk::cpp;

class Calibration {
	
	bool calib_running;
	const int plot_show_duration = 1;
	
	ofxTobiiAnalytics::Tracker *tracker_ptr;
	ofxTobiiAnalytics::Ticker ticker;
	ofxTobiiAnalytics::CalibrationPlot calib_plot;
	stack<Point2d> plot_positions;
	Point2d cur_point;
	
	int elapsed_sec;
	
public:
	
	Calibration() {
		reset();
	}
	
	void start(ofxTobiiAnalytics::Tracker *your_tracker) {
		
		if (your_tracker == nullptr) {
			ofLogError("Calibration aborted. Set tracker ptr before you start calibration.");
			return;
		}
		
		if (!your_tracker->isDeviceFound()) {
			ofLogError("Calibration aborted. EyeTracker not found. Call ofxTobiiAnalytics::Tracker::open(string) before start calibration.");
			return;
		}
		
		tracker_ptr = your_tracker;
		
		ofAddListener(ofEvents().draw, this, &Calibration::draw);
		ofAddListener(ticker.tick, this, &Calibration::onTick);
		
		reset();
		ticker.start();
		tracker_ptr->startCalibration();
		
		ofSetFullscreen(true);
		ofHideCursor();
		
		calib_running = true;
	}
	
	bool isRunning() const {
		return calib_running;
	}
	
private:
	
	void onTick(int &sec) {
		
		elapsed_sec++;
		
		if (elapsed_sec > plot_show_duration) {
			
			if (plot_positions.empty()) {
				finish();
			}
			else {
				elapsed_sec = 0;
				moveToNextPoint();
			}
		}
	}
	
	void reset() {
		
		elapsed_sec = 0;
		calib_running = false;
		
		while (!plot_positions.empty())
			plot_positions.pop();
		
		plot_positions.push(Point2d(0.9, 0.9));
		plot_positions.push(Point2d(0.1, 0.9));
		plot_positions.push(Point2d(0.5, 0.5));
		plot_positions.push(Point2d(0.9, 0.1));
		plot_positions.push(Point2d(0.1, 0.1));
		
		calib_plot.set(ofRandom(ofGetWidth()),
					   ofRandom(ofGetHeight()));
	}
	
	void draw(ofEventArgs &e) {
		calib_plot.draw();
	}
	
	void moveToNextPoint() {
		
		cout << __PRETTY_FUNCTION__ << endl;
		
		cur_point = plot_positions.top();
		
		calib_plot.set(cur_point.x, cur_point.y);
		tracker_ptr->addCalibrationPoint(cur_point);
		
		plot_positions.pop();
	}
	
	void finish() {
		
		cout << __PRETTY_FUNCTION__ << endl;
		
		ofSetFullscreen(false);
		
		ofRemoveListener(ofEvents().draw, this, &Calibration::draw);
		ofRemoveListener(ticker.tick, this, &Calibration::onTick);
		ticker.stop();
		
		// tracker
		tracker_ptr->stopCalibration();
		
		ofShowCursor();
		
		calib_running = false;
	}
};

END_OFX_TOBII_ANALYTIKS_NAMESPACE