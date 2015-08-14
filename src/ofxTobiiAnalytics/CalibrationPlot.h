#pragma once

BEGIN_OFX_TOBII_ANALYTIKS_NAMESPACE

class CalibrationPlot : public ofVec2f {
	
public:
	
	void draw() {
		ofPushStyle();
		ofPushMatrix();
		{
			ofSetColor(30);
			ofSetLineWidth(2);
			ofTranslate(x * ofGetWidth(), y * ofGetHeight());
			
			ofFill();
			ofDrawCircle(0, 0, 10);
			
			ofNoFill();
			ofDrawCircle(0, 0, 30);
		}
		ofPopMatrix();
		ofPopStyle();
	}
};

END_OFX_TOBII_ANALYTIKS_NAMESPACE