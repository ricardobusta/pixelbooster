#ifndef PB_MATH_H
#define PB_MATH_H

#include <QColor>
#include <cmath>

template <typename T>
T clamp(const T &v, const T &lower, const T &upper) {
  return (std::max)(lower, (std::min)(upper, v));
}

QColor ColorLerp(QColor &c1, QColor &c2, float t) {
  QColor out;
  out.setRedF(clamp<float>(c1.redF() * (1 - t) + c2.redF() * t, 0.0f, 1.0f));
  out.setGreenF(
      clamp<float>(c1.greenF() * (1 - t) + c2.greenF() * t, 0.0f, 1.0f));
  out.setBlueF(clamp<float>(c1.blueF() * (1 - t) + c2.blueF() * t, 0.0f, 1.0f));
  out.setAlphaF(
      clamp<float>(c1.alphaF() * (1 - t) + c2.alphaF() * t, 0.0f, 1.0f));
  return out;
}

#endif  // PB_MATH_H
