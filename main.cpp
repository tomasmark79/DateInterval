//MIT License
//
//Copyright (c) 2024 Tomas Mark
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#include <iostream>
#include <ctime>
#include <limits>
#include <ios>
#include <string>
#include <iomanip>

// #define DEBUG_DATE_ANOMALIES // if enabled - you can test dates in to the past time ;-)

bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

bool isFirstDateParameterOlder(tm dateFirst, tm dateSecond)
{
    if (dateFirst.tm_year > dateSecond.tm_year)
    {
        return true;
    }
    else if (dateFirst.tm_year < dateSecond.tm_year)
    {
        return false;
    }

    if (dateFirst.tm_mon > dateSecond.tm_mon)
    {
        return true;
    }
    else if (dateFirst.tm_mon < dateSecond.tm_mon)
    {
        return false;
    }

    if (dateFirst.tm_mday > dateSecond.tm_mday)
    {
        return true;
    }
    else if (dateFirst.tm_mday < dateSecond.tm_mday)
    {
        return false;
    }

    if (dateFirst.tm_hour > dateSecond.tm_hour)
    {
        return true;
    }
    else if (dateFirst.tm_hour < dateSecond.tm_hour)
    {
        return false;
    }

    if (dateFirst.tm_min > dateSecond.tm_min)
    {
        return true;
    }
    else if (dateFirst.tm_min < dateSecond.tm_min)
    {
        return false;
    }

    return dateFirst.tm_sec > dateSecond.tm_sec;
}

tm getUserDate(std::string msg)
{
    int months[12] = {31,-1,31,30,31,30,31,31,30,31,30,31};

    time_t now_date = time(0);
    tm t = *localtime(&now_date);
#ifndef DEBUG_DATE_ANOMALIES
    tm now = *localtime(&now_date);
#endif
    char tecka;
    do
    {
        std::cout << msg;
        std::cin >> t.tm_mday >> tecka >> t.tm_mon >> tecka >> t.tm_year;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        t.tm_year   -= 1900; /* magic number */
        t.tm_mon    -= 1;
        t.tm_hour   = 0;
        t.tm_min    = 0;
        t.tm_sec    = 0;

        // set the correct size for February dependent on Leap Year occurence
        if (isLeapYear(t.tm_year))
            months[1] = 29;
        else months[1] = 28;

        if (t.tm_mday < 1 || t.tm_mday > months[t.tm_mon])
            std::cout << "Wrong date entered. (day) " << std::endl;
        else if (t.tm_mon < 0 || t.tm_mon > 11 )
            std::cout << "Wrong date entered. (month) " << std::endl;
        else if (t.tm_year < 1 || t.tm_year > 500)
            std::cout << "Wrong date entered. (year) " << std::endl;
#ifndef DEBUG_DATE_ANOMALIES
        else if (!isFirstDateParameterOlder(t, now))
            std::cout << "Wrong date entered. (the past) " << std::endl;
#endif
        else
            break; // valid input received
    }
    while (true);
    return t;
}



#ifdef DEBUG_DATE_ANOMALIES
void printDateTm(tm& date_tm)
{
    char str_now_date[128];
    strftime(str_now_date, sizeof(str_now_date), "%d.%m.%Y", &date_tm);
    std::cout << str_now_date << std::endl;
}
#endif

int main()
{
    std::cout << "Date Interval in hope of educational purpose." << std::endl;
    std::cout << "Copyright (c) 2024 Tomas Mark" << std::endl << std::endl;

#ifdef DEBUG_DATE_ANOMALIES
    time_t now_date = time(0);
    tm now_date_tm {};
    now_date_tm = getUserDate("Enter date in format d.m.Y (ex. 1.1.2039): ");
    now_date = mktime(&now_date_tm);
    printDateTm(now_date_tm); // for debug purpose
#else
    time_t now_date = time(0);
    tm now_date_tm = *localtime(&now_date);
    now_date = mktime(&now_date_tm);
#endif
    time_t user_date;
    tm user_date_tm {};
    user_date_tm = getUserDate("Enter date in format d.m.Y (ex. 1.1.2039): ");
    user_date = mktime(&user_date_tm);

    char str_user_date[128];
    strftime(str_user_date, sizeof(str_user_date), "%d.%m.%Y", &user_date_tm);

    double days_in_interval = (difftime(user_date, now_date) / (60*60*24) + 1);

    std::cout << "There are " << std::setprecision(1) << std::fixed << days_in_interval << " days until the set date of "
              << str_user_date << "." << std::endl;

    return 0;
}
