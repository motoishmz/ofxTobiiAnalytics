#pragma once
#include "ofEvents.h"
#include "ofVec3f.h"

BEGIN_OFX_TOBII_ANALYTIKS_NAMESPACE

class GazeEvent : public ofEventArgs {
	
public:
	
	static ofEvent <GazeEvent> events;
	
	uint64_t timestamp;
	
	ofVec3f leftEyePosition3d;
	ofVec3f rightEyePosition3d;
	
	uint32_t leftValidity;
	uint32_t rightValidity;
	
	ofVec3f leftEyePosition3dRelative;
	ofVec3f rightEyePosition3dRelative;
	
	ofVec3f leftGazePoint3d;
	ofVec3f rightGazePoint3d;
	
	ofVec2f leftGazePoint2d;
	ofVec2f rightGazePoint2d;
	
	float leftPupilDiameter;
	float rightPupilDiameter;
};

ofEvent <GazeEvent> GazeEvent::events;

END_OFX_TOBII_ANALYTIKS_NAMESPACE