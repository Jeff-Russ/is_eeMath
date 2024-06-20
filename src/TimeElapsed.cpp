#include "TimeElapsed.h"


TimeElapsed::TimeElapsed(TimeElapsed_t start_now, TimeElapsed_t(*time_func)()): _time_func{time_func} {
	if (start_now) clearAndStart(start_now);
}

void TimeElapsed::clearAndStart(TimeElapsed_t time_now) {
	if (time_now > 1) _tn = time_now;
	else _tn = (*_time_func)();
	_t0 = _t1 = _tn;
	_prev_elapsed = _tn_minus_t1 = _elapsed = _elapsed_overflows = 0;
	_state = STARTED;
}

TimeElapsed_t TimeElapsed::elapsed(TimeElapsed_t update) {
	if (_state == STOPPED) return 0;

	if (_state == PAUSED || update == 0) {
		if (_elapsed_overflows) return 0;
		else return _elapsed;
	}
	
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

TimeElapsed_t TimeElapsed::pause(bool update) {
	if (_state < STARTED) return 0;

	TimeElapsed_t ret;
	if (update) ret = elapsed(true);
	else if (_elapsed_overflows) ret = 0;
	else ret = _elapsed;

	_state = PAUSED;
	return ret;
}

TimeElapsed_t TimeElapsed::resume(TimeElapsed_t time_now) {
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

TimeElapsed_t TimeElapsed::elapsedSinceStart() {
	if (_state < STARTED || _elapsed_overflows) return 0;
	else return _tn - _t0;
}

TimeElapsedState TimeElapsed::state() {
	return _state;
}

unsigned short TimeElapsed::_elapsedOverflows() {
	return _elapsed_overflows;
}
