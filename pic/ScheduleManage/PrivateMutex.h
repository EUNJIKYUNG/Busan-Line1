#ifndef MUTEX_H

#define MUTEX_H



#ifdef _WIN32

#include <windows.h>



/**

 * @brief Cross-platform mutex to protect the critical section.

 */

class Mutex {

private:

	/**

	 * @brief Actual mutex used by the system.

	 */

	HANDLE m;



public:

	/**

	 * @brief Constructor.

	 */

	Mutex() {

		m = CreateMutex(NULL, FALSE, NULL);

	}



	/**

	 * @brief Desctructor.

	 */

	~Mutex() {

		CloseHandle(m);

	}



	/**

	 * @brief Attempts to lock the mutex.

	 */

	bool try_lock() {

		return WaitForSingleObject(m, 0) == WAIT_OBJECT_0;

	}



	/**

	 * @brief Locks the mutex.

	 */

	void lock() {

		WaitForSingleObject(m, INFINITE);

	}



	/**

	 * @brief Unlocks the mutex.

	 */

	void unlock() {

		ReleaseMutex(m);

	}

};

#else

#include <pthread.h>



/**

 * @brief Cross-platform mutex to protect the critical section.

 */

class Mutex {

private:

	/**

	 * @brief Actual mutex used by the system.

	 */

	pthread_mutex_t m;



public:

	/**

	 * @brief Constructor.

	 */

	Mutex() {

		pthread_mutex_init(&m, NULL);

	}



	/**

	 * @brief Desctructor.

	 */

	~Mutex() {

		pthread_mutex_destroy(&m);

	}



	/**

	 * @brief Attempts to lock the mutex.

	 */

	bool try_lock() {

		return pthread_mutex_trylock(&m) == 0;

	}



	/**

	 * @brief Locks the mutex.

	 */

	void lock() {

		pthread_mutex_lock(&m);

	}



	/**

	 * @brief Unlocks the mutex.

	 */

	void unlock() {

		pthread_mutex_unlock(&m);

	}

};

#endif



#endif