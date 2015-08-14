#include "ofMain.h"
#include "ofxTobiiAnalytics.h"


class ofApp : public ofBaseApp
{
	
	ofxTobiiAnalytics::Tracker tracker;
	ofxTobiiAnalytics::Calibration calib;
	
	const int path_limit = 50;
	deque<ofPoint> path;
	ofPoint gaze_pos;
	
public:
	
	void setup()
	{
		ofSetFrameRate(60);
		ofSetVerticalSync(true);
		ofSetFullscreen(true);

		path.clear();
		
		ofAddListener(ofxTobiiAnalytics::GazeEvent::events, this, &ofApp::gazeDataReceived);
	}
	
	void update()
	{
		if (path.size() >= path_limit)
			path.pop_back();
	}
	
	void draw()
	{
		if (tracker.isTracking() && !path.empty())
		{
			ofSetColor(0, 80);
			
			for (int i=1; i<path.size(); i++)
				ofDrawLine(path.at(i-1), path.at(i));
			
			ofDrawCircle(path.front(), 30);
		}
		
		if (!tracker.isDeviceFound()) {
			ofDrawBitmapStringHighlight("Press [ spacebar ] to connect", 30, 30, ofColor::red);
		}
	}
	
	void keyPressed(int key)
	{
		if (key == ' ') {
			
			if (tracker.isTracking())
				return;
			
			/*!
				Without device id: Auto detection --> startTracking
				Sometimes the EyeTrackerBrowser cannot find any devices, so calling this in setup() is not recommended.
			 */
			tracker.open();
			
			/*!
				With device id: looking up the specific device --> startTracking
				Stable.
			 */
			// tracker.open("X230C-XXXXXXXXXXXX");
		}
		
//		if (key == 'c') {
//			/*!
//				Danger!!!
//				ofxTobiiAnalytics::Calibration doesn't work correctly. See readme.md
//			 */
//			calib.start(&tracker);
//		}
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
		
//		gaze_pos = mean;
		gaze_pos = (mean * 0.3) + (gaze_pos * (1 - 0.3)); //! in case you want to make it a bit smoothly...
		
		path.push_front(gaze_pos);
	}
};



#pragma mark -
#pragma mark main
int main(){
	ofSetupOpenGL(1600, 900, OF_WINDOW);
	ofRunApp(new ofApp());
}
