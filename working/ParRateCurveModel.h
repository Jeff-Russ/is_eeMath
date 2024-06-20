#include "github-nasa-polyfit/PolyFit.cpp"

template <typename T>
class ParRateCurveModel {
 public:
  T multiplier;
  T * coeffs_stored;
  int coeffs_stored_size;
  T * v_rate_lookup;
  int v_rate_lookup_size;

  ParRateCurveModel(T multiplier, T * coeffs_stored, int coeffs_stored_size) {

    ///NOTE: Eventually we may incorporate the circuit sampling / EEPROM data handling tasks into this

    this->multiplier = multiplier;
    this->coeffs_stored_size = coeffs_stored_size;
    this->coeffs_stored = new int[coeffs_stored_size];
    memcpy(this->coeffs_stored, coeffs_stored, coeffs_stored_size * sizeof(T)); // instead of:
    // for (int i = 0; i < this->coeffs_stored; ++i) this->coeffs_stored[i] = coeffs_stored[i];
  }
};