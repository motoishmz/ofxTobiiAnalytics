# OpenFrameworks addon for Tobii Analytics SDK 3.0 

	!!!: This addon doesn't support OF less than 0.8.4 
	!!!: See Known Issue section before use this addon

## Installation

Download Tobii Analytics SDK 3.0 from [here](http://www.tobii.com/en/eye-tracking-research/global/landingpages/analysis-sdk-30/). 

Copy these two directory

-  `{SDKRoot}/cpp/include` 
-  `{SDKRoot}/cpp/lib` 

to...

- `ofxTobiiAnalytics/libs/Tobii`

![ ](https://40.media.tumblr.com/10318d6e95b488fed7a651bf68624055/tumblr_nt2hxoNHXL1s2up8jo1_1280.png)


## Dependencies

- Boost (run `$ brew install boost` on Terminal)



## Building from scratch

	The version of Boost used in the example project is `1.57.0`. Change the version number as your environment.

### Header search path

- ../../../addons/ofxTobiiAnalytics/libs/Tobii/include
- Boost

![](https://41.media.tumblr.com/96942a9ad380826d7c00b85917ea9198/tumblr_nt2hxoNHXL1s2up8jo2_1280.png)

### library search path

- ../../../addons/ofxTobiiAnalytics/libs/Tobii/lib
- Boost

![](https://41.media.tumblr.com/5f4380257627413930173e1379253eb0/tumblr_nt2hxoNHXL1s2up8jo3_1280.png)

### Other linker flag

- -lboost_system
- -lboost\_program\_options
- -lboost_thread-mt

![](https://40.media.tumblr.com/db60187c856038f061295d49323f2f2c/tumblr_nt2hxoNHXL1s2up8jo4_1280.png)

### Runpath search paths

- @loader_path/../Frameworks

![](https://41.media.tumblr.com/a1671e51f75bf9803bbea13839030d3c/tumblr_nt2hxoNHXL1s2up8jo5_1280.png)

Make sure the libtetio.dylib is added to `BuildPhase > Copy Files`

![](https://41.media.tumblr.com/1465033627d35b6108a1dcecf498c005/tumblr_nt2hxoNHXL1s2up8jo6_1280.png)



## Known Issue

Calibration class doesn't work correctly since this addon is very alpha. Use the [example app](http://cl.ly/083z401V0F27) included in SDK/Sample directory instead of `ofxTobiiAnalytics::Calibration` for quick calibration.






