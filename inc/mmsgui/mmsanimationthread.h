/***************************************************************************
 *   Copyright (C) 2005-2007 Stefan Schwarzer, Jens Schneider,             *
 *                           Matthias Hardt, Guido Madaus                  *
 *                                                                         *
 *   Copyright (C) 2007-2008 BerLinux Solutions GbR                        *
 *                           Stefan Schwarzer & Guido Madaus               *
 *                                                                         *
 *   Copyright (C) 2009      BerLinux Solutions GmbH                       *
 *                                                                         *
 *   Authors:                                                              *
 *      Stefan Schwarzer   <stefan.schwarzer@diskohq.org>,                 *
 *      Matthias Hardt     <matthias.hardt@diskohq.org>,                   *
 *      Jens Schneider     <pupeider@gmx.de>,                              *
 *      Guido Madaus       <guido.madaus@diskohq.org>,                     *
 *      Patrick Helterhoff <patrick.helterhoff@diskohq.org>,               *
 *      René Bählkow       <rene.baehlkow@diskohq.org>                     *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License version 2.1 as published by the Free Software Foundation.     *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA            *
 **************************************************************************/

#ifndef MMSANIMATIONTHREAD_H_
#define MMSANIMATIONTHREAD_H_

#include "mmstools/mmstools.h"

// size of the times buffer needed for calculation of the CPU average
#define MMSANIMATIONTHREAD_TIMES_BUF_SIZE 10

//! This class helps the MMSGUI and user specific applications to get smooth animations.
/*!
There are three callbacks: onBeforeAnimation, onAnimation and onAfterAnimation
At least the onAnimation callback should be connected. This callback will be called
for each frame which is to draw.
The onBeforeAnimation callback can be used to setup something.
The onAfterAnimation callback will be called, if the animation is finished.
The initiator can be blocked during the animation (see start(false)). But it is also
possible that the animation is running in a separate thread (see start(true)).
\author Jens Schneider
*/
class MMSAnimationThread : public MMSThread {
    private:
    	//! recalculation requested?
    	bool	recalc_requested;

        //! after X loops, recalculate animation values
    	int		recalc_interval;

    	//! recalc loop counter
        int		recalc_cnt;

        //! current step length
    	int		step_len;

    	//! animation steps per second
    	int		steps_per_second;

        //! maximum cpu usage in percent
    	int		max_cpu_usage;

    	//! duration of the onAnimation callback
    	int		process_time;

    	//! delay in milliseconds between two frames
    	int		frame_delay;

    	//! maximum frame rate
    	int		max_frame_rate;

    	//! current frame rate
    	int		frame_rate;

    	//! ring buffer to calculate the CPU average
    	unsigned int times_buf[MMSANIMATIONTHREAD_TIMES_BUF_SIZE];

    	//! next pos in the ring buffer to write to
    	unsigned int times_buf_pos;

    	//! filling level of the ring buffer
    	unsigned int times_buf_cnt;

    	//! start timestamp of the animation
    	unsigned int anim_start;

    	//! end timestamp of the animation
    	unsigned int anim_end;

    	//! duration in milliseconds of the animation
    	unsigned int duration;

    	//! Internal method: Reset all values.
    	void reset();

    	//! Internal method: Main method for the animation timing control.
		void threadMain();

    public:
    	//! The constructor.
    	MMSAnimationThread();

    	//! The destructor.
    	~MMSAnimationThread();

        //! Start the animation.
        /*!
        \param separate_thread	true:  a new thread will be started
                                false: the animation is running in callers thread context
                                       and the start() method will return if the animation is finished
        */
		void start(bool separate_thread = true);

        //! Set animation steps per second.
        /*!
        \param steps_per_second 1..255 steps per second, default are 25 steps
        \return true, if parameter is accepted
        */
		bool setStepsPerSecond(int steps_per_second);

        //! Get steps per second.
        /*!
        \return steps per second
        */
		int getStepsPerSecond();

        //! Set maximum CPU usage.
        /*!
        \param max_cpu_usage 10..100 percent, default is 75%
        \return true, if parameter is accepted
        */
		bool setMaxCPUUsage(int max_cpu_usage);

        //! Get maximum CPU usage.
        /*!
        \return max CPU usage
        */
		int getMaxCPUUsage();

        //! Set maximum frame rate.
        /*!
        \param max_frame_rate 10..100 frames per second, default are 25 fps
        \return true, if parameter is accepted
        */
		bool setMaxFrameRate(int max_frame_rate);

        //! Get maximum frame rate.
        /*!
        \return max frame rate
        */
		int getMaxFrameRate();

		//! Get the current frame rate.
        /*!
        \return current frame rate
        \note This value is valid during the animation (e.g. in the onAnimation callback).
        */
		int getFrameRate();

		//! Get the delay in milliseconds between two frames.
        /*!
        \return delay in milliseconds
        \note This value is valid during the animation (e.g. in the onAnimation callback).
        */
		int getFrameDelay();

		//! Get the length of the current animation step.
        /*!
        \return length of the step
        \note This value is valid during the animation (e.g. in the onAnimation callback).
        */
		int getStepLength();

        //! Get the duration in milliseconds of the animation.
        /*!
        \return length of the step
        \note This value is valid after the animation (e.g. in the onAfterAnimation callback).
        */
		unsigned int getDuration();

        //! Set one or more callbacks for the onBeforeAnimation event.
        /*!
        The connected callbacks will be called during threadMain().
        If at least one of the callbacks returns false, the animation will be stopped.

        A callback method must be defined like this:

        	bool myclass::mycallbackmethod(MMSAnimationThread *animThread);

        	Parameters:

        		animThread -> is the pointer to the caller (the animation thread)

        	Returns:

        		true if the animation should continue, else false if the animation should stop

        To connect your callback to onBeforeAnimation do this:

            sigc::connection connection;
            connection = myanimationthread->onBeforeAnimation.connect(sigc::mem_fun(myobject,&myclass::mycallbackmethod));

        To disconnect your callback do this:

            connection.disconnect();

        Please note:

            You HAVE TO disconnect myobject from onBeforeAnimation BEFORE myobject will be deleted!!!
            Else an abnormal program termination can occur.
            You HAVE TO call the disconnect() method of sigc::connection explicitly. The destructor will NOT do this!!!
        */
        sigc::signal<bool, MMSAnimationThread*>::accumulated<bool_accumulator> onBeforeAnimation;

        //! Set one or more callbacks for the onAnimation event.
        /*!
        The connected callbacks will be called during threadMain().
        If at least one of the callbacks returns false, the animation will be stopped.

        A callback method must be defined like this:

        	bool myclass::mycallbackmethod(MMSAnimationThread *animThread);

        	Parameters:

        		animThread -> is the pointer to the caller (the animation thread)

        	Returns:

        		true if the animation should continue, else false if the animation should stop

        To connect your callback to onAnimation do this:

            sigc::connection connection;
            connection = myanimationthread->onAnimation.connect(sigc::mem_fun(myobject,&myclass::mycallbackmethod));

        To disconnect your callback do this:

            connection.disconnect();

        Please note:

            You HAVE TO disconnect myobject from onAnimation BEFORE myobject will be deleted!!!
            Else an abnormal program termination can occur.
            You HAVE TO call the disconnect() method of sigc::connection explicitly. The destructor will NOT do this!!!
        */
        sigc::signal<bool, MMSAnimationThread*>::accumulated<bool_accumulator> onAnimation;

        //! Set one or more callbacks for the onAfterAnimation event.
        /*!
        The connected callbacks will be called during threadMain().

        A callback method must be defined like this:

        	void myclass::mycallbackmethod(MMSAnimationThread *animThread);

        	Parameters:

        		animThread -> is the pointer to the caller (the animation thread)

        To connect your callback to onAfterAnimation do this:

            sigc::connection connection;
            connection = myanimationthread->onAfterAnimation.connect(sigc::mem_fun(myobject,&myclass::mycallbackmethod));

        To disconnect your callback do this:

            connection.disconnect();

        Please note:

            You HAVE TO disconnect myobject from onAfterAnimation BEFORE myobject will be deleted!!!
            Else an abnormal program termination can occur.
            You HAVE TO call the disconnect() method of sigc::connection explicitly. The destructor will NOT do this!!!
        */
		sigc::signal<void, MMSAnimationThread*> onAfterAnimation;
};

#endif /*MMSANIMATIONTHREAD_H_*/
