#include <QString>
#include <QtTest>
#include "pb_math.h"
#include <exception>

class ColorLerpTest : public QObject
{
    Q_OBJECT

public:
    ColorLerpTest()
        : blackColor(0, 0, 0, 255), whiteColor(255, 255, 255, 255) {}
private:
    QColor blackColor;
    QColor whiteColor;

private Q_SLOTS:
    void test_lerp_to_same_color_should_return_same_color();
    void test_lerp_with_zero_interpolation_should_return_first_color();
    void test_lerp_with_full_interpolation_should_return_second_color();
    void test_lerp_with_interpolation_less_than_zero_should_clamp_to_first_color();
    void test_lerp_with_interpolation_greater_than_one_should_not_clamp_to_second_color();
};

void ColorLerpTest::test_lerp_to_same_color_should_return_same_color()
{
    QColor lerpedColor =  ColorLerp(this->whiteColor, this->whiteColor, 0.5);
    QVERIFY(lerpedColor == this->whiteColor);
}

void ColorLerpTest::test_lerp_with_zero_interpolation_should_return_first_color()
{
    QColor lerpedColor =  ColorLerp(this->blackColor, this->whiteColor, 0.0);
    QVERIFY(lerpedColor == this->blackColor);
}

void ColorLerpTest::test_lerp_with_full_interpolation_should_return_second_color()
{
    QColor lerpedColor =  ColorLerp(this->blackColor, this->whiteColor, 1.0);
    QVERIFY(lerpedColor == this->whiteColor);
}

void ColorLerpTest::test_lerp_with_interpolation_greater_than_one_should_not_clamp_to_second_color()
{
    QColor lerpedColor =  ColorLerp(this->blackColor, this->whiteColor, 2.0);
    QVERIFY(lerpedColor == this->whiteColor);
}

void ColorLerpTest::test_lerp_with_interpolation_less_than_zero_should_clamp_to_first_color()
{
    QColor lerpedColor =  ColorLerp(this->blackColor, this->whiteColor, -1.0);
    QVERIFY(lerpedColor == this->blackColor);
}

QTEST_APPLESS_MAIN(ColorLerpTest)

#include "tst_colorlerp.moc"
