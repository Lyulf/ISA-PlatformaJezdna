#include "Utils.h"

/* Funkcja zwracająca kąt który samochód musi się obrócić na podstawie
 jeśli difference(diff*/
double angleDifference(double current, double target) {
  double diff = target - current;
  if (diff > 180) {
    diff = diff - 360;
  }
  else if (diff < -180) {
    diff = diff + 360;
  }
  return diff;
}