/**
 * used for creating eased transitions
 *
 * by Blakely L McConnell  2015->~∞
 */

#pragma once

#include "cinder/CinderMath.h"
#include "cinder/Easing.h"
#include "boost/signals2.hpp"

namespace drifter
{
namespace utility
{
    typedef boost::signals2::signal< void() > TimedEvent;
    class TimedTransition
    {
    private:
        bool        _bIsActive;     //if the timer is active, elapsed time will increment
        bool        _bEventFired;   //an event is fired somewhere during the transition
        float       _lastTime;      //keep track of app elapsed time from last check
        float       _timeDiff;      //difference between current app elapsed time and last time
        float       _rate;          //rate (in ms) to update current count
        float       _pivot;         //percentage complete at which to end fade out
        float       _wait;          //percentage of time to wait before fading in
        float       _eventCue;      //percentage of time elapsed to fire event
        int         _currentCount;  //current step in transition
        int         _totalCount;    //total steps in transition
        TimedEvent  _timedEvent;    //signal to fire transition event

        float Ease( const float elapsed )
        {
            float retVal;

            //map output and ease
            //TODO magic numbers here should be exposed paramaters
            if ( elapsed <= _pivot ) {
                retVal = ci::lmap( elapsed, 0.0f, 0.4f, 0.0f, 1.0f );
                retVal = ci::easeOutQuad( retVal );
            }
            else if ( elapsed >= ( _pivot + _wait )) {
                retVal = ci::lmap( elapsed, 0.5f, 1.0f, 1.0f, 0.0f );
                retVal = ci::easeInQuad( retVal );
            }
            else {
                retVal = 1.0f;
            }

            return retVal;
        }

    public:
        //TODO expose these parameters
        TimedTransition()
        {
            _bIsActive = false;
            _bEventFired = false;
            _lastTime = 0.0f;
            _currentCount = 0.0f;
            _pivot = 0.4f;
            _wait = 0.1f;
            _eventCue = 0.4f;
            _rate = 30.0f;
            _totalCount = 100.0f;
        }

        bool EventFired()                       { return _bEventFired; }
        void SetActive()                        { _bIsActive = true; }
        TimedEvent & Event()                    { return _timedEvent; }

        float Update()
        {
            float retVal;

            //if timer is active return a float between 0 and 1
            if ( _bIsActive ) {

                //get the difference between current app time and last time count was incremented
                float currentTime = ci::app::getElapsedSeconds() * 1000;
                _timeDiff = currentTime - _lastTime;
                if ( _timeDiff >= _rate     ) {        //if greater than threshold increment counter
                    _currentCount += 1.0f;
                    _lastTime = currentTime;
                }

                //check for event firing elapsed time
                if ( _currentCount > ( _totalCount * _eventCue )  && !_bEventFired ) {
                    _timedEvent();
                    _bEventFired = true;
                }

                //if the count exceeds the total count, reset everything
                if ( _currentCount >= _totalCount ) {
                    retVal = -1;
                    _currentCount = 0.0f;
                    _bIsActive = false;
                    _bEventFired = false;
                }

                //return the percentage of transition completion
                else {
                    float elapsed = (float)_currentCount / (float)_totalCount;
                    return Ease( elapsed );
                }
                return retVal;
            }
            //return -1 if the timer is not active
            else
                return -1;
        }
    };
} //utility
} //drifter

#endif
