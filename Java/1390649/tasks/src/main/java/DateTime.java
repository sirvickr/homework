public class DateTime {
    private static long ticksPerSecond = 10000000;
    private static long ticksPerMinute = ticksPerSecond * 60;
    private static long ticksPerHour = ticksPerMinute * 60;
    private static long ticksPerDay = ticksPerHour * 24;
    private static long ticksPerWeek = ticksPerDay * 7;
    private static long TicksPerHalfMilliSecond = 5000;
    private static int[] DaysToMonth365 = new int[]{ 0, 0x1f, 0x3b, 90,   120,  0x97, 0xb5, 0xd4, 0xf3, 0x111, 0x130, 0x14e, 0x16d };
    private static int[] DaysToMonth366 = new int[]{ 0, 0x1f, 60,   0x5b, 0x79, 0x98, 0xb6, 0xd5, 0xf4, 0x112, 0x131, 0x14f, 0x16e };

    DateTime() {
        dateData = 0;
    }

    DateTime(int year, int month, int day, int hour, int minute, int second)
    {
        dateData = (long) (DateToTicks(year, month, day) + TimeToTicks(hour, minute, second, 0));
    }

    void Add(DateTime dt) {
        dateData += dt.GetTicks();
    }

    void AddWeeks(int value) {
        dateData += value * ticksPerWeek;
    }

    void AddDays(int days) {
        dateData += days * ticksPerDay;
    }

    void AddHours(int hours) {
        dateData += hours * ticksPerHour;
    }

    void AddMinutes(int minutes) {
        dateData += minutes * ticksPerMinute;
    }

    void AddSeconds(int seconds) {
        dateData += seconds * ticksPerSecond;
    }

    void Sub(DateTime dt) {
        dateData -= dt.GetTicks();
    }

    // Gets time expressed in 100-nanosecod units.
    long GetTicks() { return ((long)dateData) & 0x3fffffffffffffffl; }

    // Gets the year represented by this instance (1 through 9999).
    int GetYear()      { return GetDatePart(0); }
    /// Gets the month represented by this instance (1 through 12).
    int GetMonth()     { return GetDatePart(2); }
    /// Gets the day of month represented by this instance (1 through number of days in month).
    int GetDay()       { return GetDatePart(3); }
    /// Gets the day of the year represented by this instance (1 through 366).
    int GetDayOfYear() { return GetDatePart(1); }
    /// Gets the day of the week represented by this instance.
    /// Return value ranges from zero, indicating Sunday, to six, indicating Saturday.
    int GetDayOfWeek() { return ((int) ((((GetTicks() + TicksPerHalfMilliSecond) / 0xc92a69c000l) + 1) % ((long) 7))); }

    /// Gets the hour component of the date represented by this instance (0 through 23).
    int GetHour()        { return (int) (((GetTicks() + TicksPerHalfMilliSecond) / 0x861c46800l) % ((long) 0x18)); }
    /// Gets the minute component of the date represented by this instance (0 through 59).
    int GetMinute()      { return (int) (((GetTicks() + TicksPerHalfMilliSecond) / ((long) 0x23c34600)) % ((long) 60)); }
    /// Gets the second component of the date represented by this instance (0 through 59).
    int GetSecond()      { return (int) (((GetTicks() + TicksPerHalfMilliSecond) / ((long) 0x989680)) % ((long) 60)); }
    /// Gets the millisecond component of the date represented by this instance (0 through 999).
    int GetMillisecond() { return (int) (((GetTicks() + TicksPerHalfMilliSecond) / ((long) 0x2710)) % ((long) 0x3e8)); }

    int GetDatePart(int part) {
        int num2 = (int) (GetTicks() / 0xc92a69c000l);
        int num3 = num2 / 0x23ab1;
        num2 -= num3 * 0x23ab1;
        int num4 = num2 / 0x8eac;
        if (num4 == 4)
        {
            num4 = 3;
        }
        num2 -= num4 * 0x8eac;
        int num5 = num2 / 0x5b5;
        num2 -= num5 * 0x5b5;
        int num6 = num2 / 0x16d;
        if (num6 == 4)
        {
            num6 = 3;
        }
        if (part == 0)
        {
            return (((((num3 * 400) + (num4 * 100)) + (num5 * 4)) + num6) + 1);
        }
        num2 -= num6 * 0x16d;
        if (part == 1)
        {
            return (num2 + 1);
        }
        int num7 = num2 >> 6;
        int[] numArray1 = ((num6 == 3) && ((num5 != 24) || (num4 == 3))) ? DaysToMonth366 : DaysToMonth365;
        while (num2 >= numArray1[num7])
        {
            num7++;
        }
        if (part == 2)
        {
            return num7;
        }
        return ((num2 - numArray1[num7 - 1]) + 1);
    }

    static long TimeToTicks(int hour, int minute, int second, int ms) {
        long seconds = ((hour * 3600) + (minute * 60)) + second;
        return (seconds * 10000000 + ms * 10000);
    }

    static long DateToTicks(int year, int month, int day)
    {
        if (((year >= 1) && (year <= 9999)) && ((month >= 1) && (month <= 12)))
        {
            int[] daysToMonth = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
            if ((day >= 1) && (day <= (daysToMonth[month] - daysToMonth[month - 1])))
            {
                long year1 = year - 1;
                long days = ((((((year1 * 365) + (year1 / 4)) - (year1 / 100)) + (year1 / 400)) + daysToMonth[month - 1]) + day) - 1;
                return (days * 0xc92a69c000l);
            }
        }
        System.out.println("'year', 'month' or 'day' has invalid value");
        return 0;
    }

    static boolean IsLeapYear(int year) {
        if ((year < 1) || (year > 9999))
        {
            System.out.println("year is out of range [1, 9999]");
            return false;
        }
        if ((year % 4) != 0)
        {
            return false;
        }
        if ((year % 100) == 0)
        {
            return ((year % 400) == 0);
        }
        return true;
    }

    private long dateData;
}
