#pragma once

BEGIN_OFX_TOBII_ANALYTIKS_NAMESPACE

class Ticker {
	
	float start_time;
	float elapsed_timef;
	int elapsed_sec;
	
	void clear() {
		start_time = 0.;
		elapsed_timef = 0.;
		elapsed_sec = 0;
	}
	
	void update(ofEventArgs &e) {
		elapsed_timef += ofGetLastFrameTime();
		
		int sec = floor(elapsed_timef);
		
		if (elapsed_sec != sec) {
			elapsed_sec = sec;
			ofNotifyEvent(tick, sec, this);
		}
	}
	
public:
	
	ofEvent<int> tick;
	
	Ticker() {
		clear();
	}
	
	void start() {
		ofAddListener(ofEvents().update, this, &Ticker::update);
		start_time = ofGetElapsedTimef();
	}
	
	void stop() {
		ofRemoveListener(ofEvents().update, this, &Ticker::update);
		clear();
	}
};

END_OFX_TOBII_ANALYTIKS_NAMESPACE