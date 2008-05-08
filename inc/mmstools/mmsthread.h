/***************************************************************************
 *   Copyright (C) 2005-2008 by                                            *
 *                                                                         *
 *      Stefan Schwarzer <sxs@morphine.tv>                                 *
 *      Guido Madaus     <bere@morphine.tv>                                *
 *      Jens Schneider   <pupeider@morphine.tv>                            *
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

#ifndef MMSTHREAD_H_
#define MMSTHREAD_H_

#include <pthread.h>
#include <sched.h>
#include "mmstools/mmslogger.h"

class MMSThread {

	private:
		int				priority;
        pthread_attr_t	tattr;
        sched_param		param;
		pthread_t 		id;
		bool 			isrunning;
		
	public:
        string    identity;

//	protected:
		void run();

	public:
		MMSThread(string identity = "MMSThread", int priority = 0);
        virtual ~MMSThread() {};
		virtual void threadMain() = 0;
		void start();
		void detach();
		bool isRunning();
		int cancel();
};

#endif /*MMSTHREAD_H_*/
