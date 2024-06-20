#pragma once

/**************************************************************************************************
* @file  TimeElapsed.h
* 
* @brief TimeElapsed constructs objects to measure time intervals.
***************************************************************************************************/

// NOTE: micros and millis return unsigned long which are 32-bit. 

#ifndef TimeElapsed_t
typedef unsigned long TimeElapsed_t; ///< should match return of time function used by TimeElapsed (unsigned long for micros and millis)
#endif

#include <Arduino.h>

// do not change the order of these!:
enum TimeElapsedState {
  STOPPED, ///< stopped state=0
  PAUSED,  ///< paused state=1
  STARTED, ///< never paused state=2
  RESUMED  // previously paused state=3
}; ///< enum for TimeElapsed obj._state with values STOPPED=0, PAUSED, STARTED, or RESUMED */


class TimeElapsed {
 public:
  TimeElapsed_t (*_time_func)(); // time function is normally micros() or millis() [Make getter/setter]

  /**
   * @brief current state as value from TimeElapsedState enum, which is one of the following:
   * STOPPED, // =0
   * PAUSED,  // =1
   * STARTED, // =2 (never paused)
   * RESUMED  // =3 (previously paused)
   */
  TimeElapsedState _state = STOPPED;

  TimeElapsed_t _t0 = 0; // the start time: return from _time_func() when transisitioned to STARTED (not RESUMED). Used to get total time, including paused time.
  TimeElapsed_t _t1 = 0; // the start or resume time, whichever is most recent (return from _time_func() when transisitioned to STARTED or RESUMED, which ever is most recent)
  TimeElapsed_t _tn = 0; // the checked time (return from (the most recent call to _time_func(), no matter the state)
  
  TimeElapsed_t _prev_elapsed = 0; // Each time we go PAUSED->RESUMED, _tn_minus_t1 is accumlated here, thus elapsed running time is _prev_elapsed + _tn_minus_t1
  TimeElapsed_t _tn_minus_t1 = 0;  // time running that elapsed between _t1 and _tn (does not include any elapsed time occuring before any unpausing. 
  // The idea is, each time we check the (running) time elapsed, first calculate: _tn=time_func(); _tn_minus_t1 = _tn - _t0; // and then:
  TimeElapsed_t _elapsed = 0; // _elapsed = _tn_minus_t1 + _prev_elapsed // this is the (running) time elapsed so it excluded any time spent paused

  unsigned short _elapsed_overflows = 0;

  /**
   * @brief constructs object to measure time intervals, with start/restart functionality, both vias 
   * .clearAndStart(),as well as pause() and resume() methods.
   * EXAMPLE: TimeElapsed<unsigned long long> te(true, millis); // or maybe: TimeElapsed<decltype(millis)> te(true, millis);
   * 
   * @param start_now (default=false=0) if true=1, triggers calling of time function and storing that time 
   * as the start of the time elapsed period. if >1, the value of this arg is used as the start time 
   * (you call time function, passing its return here, E.g; to multiple objects), else, the time is not started.
   * @param time_func (default=micros) is normally micros() or millis() */ // DONE
  TimeElapsed(TimeElapsed_t start_now=false, TimeElapsed_t(*time_func)()=micros) : _time_func{time_func} {
    if (start_now) clearAndStart(start_now);
  }

  /** 
   * @brief changes .state() to STARTED. clears stored times and sets the start time time function's return.
   * @param time_now (default=1) if >1, the value of the arg is used as the start time (you call time 
   * function, passing it's return here, E.g; to multiple objects), else, the time function  is called to get 
   * this value. */ // DONE
  void clearAndStart(TimeElapsed_t time_now=1) {
    if (time_now > 1) _tn = time_now;
    else _tn = (*_time_func)();
    _t0 = _t1 = _tn;
    _prev_elapsed = _tn_minus_t1 = _elapsed = _elapsed_overflows = 0;
    _state = STARTED;
  }
  
  /**
   * @brief check the time elapsed since last clearAndStart (or construction if true was passed)
   * excluding any time spent paused. To include that time, call .elapsedSinceStart() instead.
   * 
   * @param update (default=true=1) if =1, the elapsed time will span until now as time function
   * will be called here to update it, if >1, the value of this arg defines "now" instead of calling
   * the time function (you call it and pass the result here, E.g; to multiple objects). else (false or 
   * 0 if TimeElapsed_t is unsigned, <=0 if it is signed), only the time already accumulated up to the last call callling 
   * time function is observed, nothing since then, which is is equivalent to checking elapsed while paused.
   * 
   * @return TimeElapsed_t (usually unsigned long) if non-zero, is non-paused time elapsed until now or, if false=0 was 
   * passed, until elapsed was last checked. A return of 0 should be interpreted as an error and could mean:
   * A) the currnet .state() is STOPPED or
   * B) there was an overflow falure at some point (verify by ._elapsedOverflows() returning >0)*/ // DONE
  TimeElapsed_t elapsed(TimeElapsed_t update=true) {

    // First, take care of states that would shouldn't be in when this function would be called:

    if (_state  == STOPPED) return 0;

    if (_state == PAUSED || update == 0) {
      if (_elapsed_overflows) return 0;
      else return _elapsed;
    }
    // done.
    TimeElapsed_t tn;
    if (update > 1) tn = update;
    else tn = (*_time_func)();

    TimeElapsed_t tn_minus_t1 = tn - _t1;
    TimeElapsed_t elapsed = tn_minus_t1 + _prev_elapsed;
    
    if (elapsed < _elapsed) ++_elapsed_overflows; // this causes error
    _tn = tn;
    _tn_minus_t1 = tn_minus_t1;
    _elapsed = elapsed;
    return _elapsed_overflows ? 0 : _elapsed;
  }

  /**
   * @brief Changes .state() to PAUSED so that no new (non-paused) time is accumulated to .elapsed().
   * NOTE: pausing does not affect .elapsedSinceStart() since that includes all time elapsed while paused. 
   * NOTE: If .elapsed() is called while PAUSED, the same elapsed time previously returned from 
   * .elapsed() or .pause will be returned again.  
   * 
   * @param update (default=true=1) if =1, the elapsed time will span until now as time function
   * will be called to update it, if >1, the value of update will be used as "now" instead of calling
   * the time function (you call it and pass the result, i.e. to multiple objects). else (false or 
   * 0 if TimeElapsed_t is unsigned, <=0 if it is signed), only the time already accumulated up to the last call to 
   * time function is observed, nothing since then so it as if you paused at the same instant as your 
   * last check.
   * 
   * @return TimeElapsed_t (usually unsigned long) if non-zero, is non-paused time elapsed until now or, if false was 
   * passed, until elapsed was last checked. A return of 0 should be interpreted as an error and could mean:
   * A) calling .pause() did nothing: the state was stopped or alreay paused (verify by calling .state())
   * B) there was an overflow falure at some point (verify by ._elapsedOverflows() returning >0)*/
  TimeElapsed_t pause(bool update=true) {
    
    if (_state < STARTED) return 0;

    TimeElapsed_t ret;

    if (update) ret = elapsed(true);
    else if (_elapsed_overflows) ret = 0;
    else ret = _elapsed;

    _state = PAUSED;
    return ret;
  }
  
  /**
   * @brief moves .state() from PAUSED to RESUMED. This calls the time function so that 
   * the time between this call and the next check of elapsed time will include it.
   * @param time_now (default=1) if >1, the value of the parameter is used as the start time (you call the 
   * time function and pass the result here, E.g; to multiple objects)), else, the time function is called to
   * get this value. 
   * @return TimeElapsed_t (usually unsigned long) if non-zero, is non-paused time elapsed (prevously known,
   * not updated here) or 0 
   * A) the .state() was not PAUSED or 
   * B) there was an overflow falure at some point (verify by ._elapsedOverflows() returning >0)*/ // DONE
  TimeElapsed_t resume(TimeElapsed_t time_now=1) {
    if (update == 0) {
      if (_elapsed_overflows) return 0;
      else return _elapsed;
    }
    TimeElapsed_t tn;
    if (update > 1) tn = update;
    else tn = (*_time_func)();

    if (_state != PAUSED) return 0;

    // resume means we move all _tn_minus_t1 to _prev_elapsed
    _prev_elapsed = _prev_elapsed + _tn_minus_t1;
    _tn_minus_t1 = 0; 
    // NOTE: there is no way we just overflowed prev_elapsed because it is equal to
    // _elapsed and we already check if _elapsed overflows every time it changes. 

    // and start a new span of time starting now:
    if (time_now > 1) _t1 = _tn = time_now;
    else _t1 = _tn = (*_time_func)();

    return _elapsed_overflows ? 0 : _elapsed;
  }
  
  /**
   * @return TimeElapsed_t of the time elapsed since start until last checked (by calling elapsed(), 
   * elapsed(true), pause() or pause(true)). If return is 0, the time elapsed since start 
   * has exceeded the size of TimeElapsed_t. NOTE: A non-zero return does not guarantee that there was 
   * no overflow! */
  TimeElapsed_t elapsedSinceStart() {
    if (_state < STARTED || _elapsed_overflows) return 0;
    else return _tn - _t0;
  }


  //**** property access ************************************************

  /**
  * @brief returns current state as value from TimeElapsedState enum, which is one of the following:
  * STOPPED, // =0
  * PAUSED,  // =1
  * STARTED, // =2 (never paused)
  * RESUMED  // =3 (previously paused)
  */
  TimeElapsedState state() { return _state; }

  unsigned short _elapsedOverflows() { return _elapsed_overflows; }

};
TimeElapsed te = TimeElapsed();