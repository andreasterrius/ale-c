//
// Created by Alether on 10/26/2023.
//
#include <gtest/gtest.h>

TEST_F(DateConverterFixture, FirstDayEver){ // 1/1/1 -> 1/3/1

    gregDate->SetMonth(1);
    gregDate->SetDay(1);
    gregDate->SetYear(1);

    int absGregDate = gregDate->getAbsoluteDate();
    julDate->CalcJulianDate(absGregDate);

    EXPECT_EQ(julDate->getMonth(),1);
    EXPECT_EQ(julDate->getDay(),3);
    EXPECT_EQ(julDate->getYear(),1);

}