#pragma once

typedef uint32_t (THREADCALL lpfnThreadFunc)(void *);

class Thread
{
public:
	Thread();
	Thread(lpfnThreadFunc lpThreadFunc, void * lpParam = nullptr);

	bool isStarted() const {
		return m_thread.joinable();
	}

	bool start(lpfnThreadFunc lpThreadFunc, void * lpParam = nullptr);
	bool waitForExit();

	virtual ~Thread() {}

protected:
	std::thread m_thread;

private:
	Thread(const Thread & other); /* disable copy constructor */
};
