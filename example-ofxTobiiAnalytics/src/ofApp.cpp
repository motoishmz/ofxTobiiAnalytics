#include "ofMain.h"
#include "ofxTobiiAnalytics.h"


class ofApp : public ofBaseApp
{
	
	ofxTobiiAnalytics::Tracker tracker;
	ofxTobiiAnalytics::Calibration calib;
	
	const int path_limit = 50;
	deque<ofPoint> path;
	ofPoint current_gaze_pos;
	
public:
	
	void setup()
	{
		ofSetFrameRate(60);
		ofSetVerticalSync(true);
		
		tracker.open("Your Tobii Device ID here"); // Something like X230C-XXXXXXXXXXXX written on the back of the device
		if (tracker.isDeviceFound()) {
			ofSetFullscreen(true);
			ofAddListener(ofxTobiiAnalytics::GazeEvent::events, this, &ofApp::gazeDataReceived);
			tracker.startTracking();
		}
		
		path.clear();
	}
	
	void update()
	{
		if (path.size() >= path_limit)
			path.pop_back();
	}
	
	void draw()
	{
		if (calib.isRunning())
			return;
		
		if (tracker.isTracking() && !path.empty())
		{
			ofSetColor(0, 80);
			
			for (int i=1; i<path.size(); i++)
				ofDrawLine(path.at(i-1), path.at(i));
			
			ofDrawCircle(current_gaze_pos, 30);
		}
	}
	
	void keyPressed(int key)
	{
		if (key == 'f') {
			ofToggleFullscreen();
		}
		
		/*!
			Danger!!!
			ofxTobiiAnalytics::Calibration doesn't work correctly. See readme.md
		 
//		if (key == 'c') {
//			calib.start(&tracker);
//		}
		 */
	}
	
	void gazeDataReceived(ofxTobiiAnalytics::GazeEvent &data)
	{
		//! validity code 0 --> Found two eyes
		if (data.leftValidity != 0 || data.rightValidity != 0)
			return;
		
		ofPoint mean;
		mean = (ofPoint(data.leftGazePoint2d.x, data.leftGazePoint2d.y) +
				ofPoint(data.leftGazePoint2d.x, data.leftGazePoint2d.y)) * 0.5;
		
		//! positions of the gaze data are passed as 0. ~ 1.
		mean.x *= ofGetScreenWidth();
		mean.y *= ofGetScreenHeight();
		
//		current_gaze_pos = mean;
		current_gaze_pos = (mean * 0.3) + (current_gaze_pos * (1 - 0.3)); //! in case you want to make it a bit smoothly...
		cout << current_gaze_pos << endl;
		path.push_front(current_gaze_pos);
	}
};



#pragma mark -
#pragma mark main
int main(){
	ofSetupOpenGL(1600, 900, OF_WINDOW);
	ofRunApp(new ofApp());
}
