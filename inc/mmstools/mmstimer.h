/***************************************************************************
 *   Copyright (C) 2009      Berlinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      René Bählkow     <rbaehlkow@berlinux-solutions.de                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.        *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef MMSTIMER_H_
#define MMSTIMER_H_

#include <sigc++/sigc++.h>
#include <csignal>
extern "C"
{
#include "sys/types.h"
#include "unistd.h"
}
#include "mmstools/mmsthread.h"

/*!
 * \brief The MMSTimer provides timers.
 *
 * You can use MMSTimer for repetitive or single shot timers
 * \see MMSTimer(bool singleShot). After expiration of an interval
 * (\see start(unsigned int milliSeconds)) a signal \see timeOut emits.
 * <code>
 * 	void fn1() {printf("fn1 called\n);}
 * 	MMSTimer *repetitiveTimer = new MMSTimer();
 * 	repetitiveTimer->timeOut->connect(sigc::ptr_fun(&fn1));
 * 	// calls every 1.5 seconds fn1()
 * 	repetitiveTimer->start(1500);
 *
 * 	void fn2({printf("fn2 called\n")});
 * 	MMSTimer *singleShotTimer = new MMSTimer(true);
 * 	singleShotTimer->timeOut->connect(sigc::ptr_fun(&fn2));
 * 	// calls after 30 seconds fn2()
 * 	singleShotTimer->start(30000);
 * </code>
 */

class MMSTimer: public MMSThread
{
	public:
		/*!
		 * \brief Creates a new MMSTimer.
		 *
		 * \param singleShot    If this is set to true, the timer will
		 *                      emit only once, else it will do it
		 *                      repetitive.
		 */
		MMSTimer(bool singleShot = false);

		/*!
		 * \brief Destroys a MMSTimer.
		 */
		~MMSTimer();

		/*!
		 * \brief Starts timer.
		 *
		 * \param milliSeconds    the timer interval in milli seconds
		 */
		void start(unsigned int milliSeconds);

		/*!
		 * \brief Restarts timer.
		 *
		 * A running timer stops first and no signal emits. After this
		 * a new timer starts with the full interval set with
		 * \see start(unsigned int milliSecs). This method does nothing
		 * if it is called in the same thread where the timer is running
		 * in to avoid deadlocks.
		 */
		void restart();

		/*!
		 * \brief Stops timer.
		 *
		 * A running timer stops and no signal emits. This method does
		 * nothing if it is called in the same thread where the timer
		 * is running in to avoid deadlocks.
		 */
		void stop();

		/*!
		 * \brief This signal emits if the timer expires.
		 *
		 * Connect a function to this signal.
		 */
		sigc::signal<void> timeOut;

	private:
		bool singleShot;
		bool repeat;
		bool quit;

		__time_t secs;
		long int nSecs;

		pthread_t threadID;
		pthread_cond_t interruptCond;
		pthread_mutex_t interruptMutex;
		pthread_cond_t startCond;
		pthread_mutex_t startMutex;
		pthread_cond_t stopCond;
		pthread_mutex_t stopMutex;

		void threadMain();
		void stopWithoutCheck();
};

#endif /* MMSTIMER_H_ */