#ifndef PB_MATH_H
#define PB_MATH_H
#include <QColor>
#include <cmath>

template <typename T>
T clamp(const T &v, const T &lower, const T &upper) {
  return (std::max)(lower, (std::min)(upper, v));
}

QColor ColorLerp(QColor &c1, QColor &c2, float t);

#endif // PB_MATH_H
