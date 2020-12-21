import static org.junit.jupiter.api.Assertions.*;

class DateTimeTest {
    private int year = 2020;
    private int month = 12;
    private int day = 21;

    @org.junit.jupiter.api.Test
    void addWeeks() {
        // 4) какой месяц будет через 17 недель?
        DateTime dt = new DateTime(year, month, day, 11, 58, 0);
        dt.AddWeeks(17);
        assertEquals(4, dt.GetMonth());
    }

    @org.junit.jupiter.api.Test
    void addDays() {
        // 1) какой день недели будет через 1024 дня?
        DateTime dt = new DateTime(year, month, day, 11, 58, 0);
        dt.AddDays(1024);
        assertEquals(1, dt.GetDayOfWeek());
    }

    @org.junit.jupiter.api.Test
    void sub() {
        DateTime dt = new DateTime(year, month, day, 0, 0, 0);
        // 5) сколько дней до Нового года?
        DateTime ny = new DateTime(year, 12, 31, 23, 59, 59);
        ny.Sub(dt);
        assertEquals(1, ny.GetDay());
        // 2) сколько лет, месяцев и дней назад был День победы 9 мая 1945 года?
        DateTime vd = new DateTime(1945, 5, 9, 0, 0, 0);
        dt.Sub(vd);
        assertEquals(76, dt.GetYear());
        assertEquals(8, dt.GetMonth());
        assertEquals(15, dt.GetDay());
    }

    @org.junit.jupiter.api.Test
    void getDay() {
        // 6) ближайшая пятница 13го числа?
        DateTime fr = new DateTime(year, month, day, 11, 58, 0);
        do {
            if(fr.GetDayOfWeek() == 5 && fr.GetDay() == 13)
                break;
            fr.AddDays(1);
        } while(true);
        assertEquals(2021, fr.GetYear());
        assertEquals(8, fr.GetMonth());
        assertEquals(13, fr.GetDay());
        assertEquals(5, fr.GetDayOfWeek());
    }

    @org.junit.jupiter.api.Test
    void getDayOfWeek() {
        // 3) в какой день Вы родились?
        DateTime birthday = new DateTime(2001, 10, 18, 0, 0, 0);
        assertEquals(4, birthday.GetDayOfWeek());
    }
}