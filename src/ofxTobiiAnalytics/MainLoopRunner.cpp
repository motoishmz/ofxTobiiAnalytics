#include "MainLoopRunner.h"
#include <boost/thread.hpp>
#include <boost/make_shared.hpp>
#include <tobii/sdk/cpp/Library.hpp>

MainLoopRunner::MainLoopRunner() 
: threadStarted_(false), thread_(NULL)
{
}

MainLoopRunner::~MainLoopRunner()
{
	stop();
}

tetio::MainLoop& MainLoopRunner::getMainLoop()
{ 
	return mainLoop_; 
}

void MainLoopRunner::start()
{
	if (!thread_) {
		threadStarted_ = false;
		thread_ = new boost::thread(boost::bind(&MainLoopRunner::run, this));
		while (!threadStarted_) {
			boost::this_thread::sleep(boost::posix_time::milliseconds(0));
		}
	}
}

void MainLoopRunner::stop()
{
	if (thread_) {
		mainLoop_.quit();
		thread_->join();
		delete thread_;
		thread_ = NULL;
	}
}

void MainLoopRunner::run()
{
	threadStarted_ = true;
	mainLoop_.run();
}
