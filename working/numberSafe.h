
// https://www.scaler.com/topics/c/overflow-and-underflow-in-c/#how-to-prevent-integer-underflows



template <typename T>
bool safeToAddUInt(T a, T b){ return !(a > (T)(-1) - b); }  // Good


template <typename T>
bool safeToAddInt(T a, T b){ return !((b > 0) && (a > (T)(-1) - b)); } // untested, prolly fine

