#pragma once
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <tobii/sdk/cpp/MainLoop.hpp>

namespace tetio = tobii::sdk::cpp;

// Thread hosting the main loop.
class MainLoopRunner : public boost::noncopyable
{
public:
	
	typedef boost::shared_ptr<MainLoopRunner> pointer_t;
	
	MainLoopRunner();
	virtual ~MainLoopRunner();
	tetio::MainLoop& getMainLoop();
	void start();
	void stop();
	

private:
	tetio::MainLoop mainLoop_;
	volatile bool threadStarted_;
	boost::thread* thread_;

	void run();
};
