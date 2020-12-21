/* Реализовать класс григорианского календаря со следующими особенностями поведения:
 * 1) каждый четвёртый год високосный
 * 2) каждый завершающий год века, если он не делится на 400, не является високосным
 * 3) для заданной даты определить день недели
 * 4) поддержка арифметических операций сложение и вычитание (дата +- дата, месяц, год)
 */

import java.util.Date;
import java.util.Calendar;
import java.util.GregorianCalendar;

public class Program {

	/*static byte weekday(int year, int month, int day) {
		if (month < 3) {
			--year;
			month += 10;
		} else
			month -= 2;
		return (byte)(((short)day + 31 * (short)month / 12 + year + year / 4 - year / 100 + year / 400) % 7);
	}*/

    public static void main(String[] args) {
		/*
		int year = 2004;
		int month = 10;
		int day = 5;
		*/
        int year = 2020;
        int month = 12;
        int day = 21;

        {
            DateTime dt = new DateTime(year, month, day, 11, 58, 0);
            System.out.println("0\tdt " + dt.GetYear() + "-" + dt.GetMonth() + "-" + dt.GetDay() + " " + dt.GetHour() + ":" + dt.GetMinute() + ":" + dt.GetSecond() + " week day = " + dt.GetDayOfWeek());
			/*
			dt.AddDays(1);
			System.out.println("dt " + dt.GetYear() + "-" + dt.GetMonth() + "-" + dt.GetDay() + " " + dt.GetHour() + ":" + dt.GetMinute() + ":" + dt.GetSecond() + " week day = " + dt.GetDayOfWeek());
			dt.AddHours(1);
			System.out.println("dt " + dt.GetYear() + "-" + dt.GetMonth() + "-" + dt.GetDay() + " " + dt.GetHour() + ":" + dt.GetMinute() + ":" + dt.GetSecond() + " week day = " + dt.GetDayOfWeek());
			dt.AddMinutes(2);
			System.out.println("dt " + dt.GetYear() + "-" + dt.GetMonth() + "-" + dt.GetDay() + " " + dt.GetHour() + ":" + dt.GetMinute() + ":" + dt.GetSecond() + " week day = " + dt.GetDayOfWeek());
			*/

            //Calendar calendar = new GregorianCalendar(year, month - 1, day);
            //Date date = calendar.getTime();
            //System.out.println("\n" + date.getDay() + ": " + date);
        }
        {   // 1) какой день недели будет через 1024 дня?
            DateTime dt = new DateTime(year, month, day, 11, 58, 0);
            dt.AddDays(1024);
            System.out.println("\n1\tdt " + dt.GetYear() + "-" + dt.GetMonth() + "-" + dt.GetDay() + " " + dt.GetHour() + ":" + dt.GetMinute() + ":" + dt.GetSecond() + " week day = " + dt.GetDayOfWeek());
        }
        {   // сколько лет, месяцев и дней назад был День победы 9 мая 1945 года?
            DateTime dt = new DateTime(year, month, day, 0, 0, 0);
            DateTime vd = new DateTime(1945, 5, 9, 0, 0, 0);
            dt.Sub(vd);
            System.out.println("\n2\t" + dt.GetYear() + " years " + dt.GetMonth() + " months " + dt.GetDay() + " days");
        }
        {   // в какой день Вы родились?
            DateTime bithday = new DateTime(2001, 10, 18, 0, 0, 0);
            System.out.println("\n3\tв какой день Вы родились: " + bithday.GetDayOfWeek());
        }
        {   // 4) какой месяц будет через 17 недель?
            DateTime dt = new DateTime(year, month, day, 11, 58, 0);
            dt.AddWeeks(17);
            System.out.println("\n1\tкакой месяц будет через 17 недель: " + dt.GetMonth());
        }
        {   // 5) сколько дней до Нового года?
            DateTime dt = new DateTime(year, month, day, 0, 0, 0);
            DateTime ny = new DateTime(year, 12, 31, 23, 59, 59);
            ny.Sub(dt);
            System.out.println("\n1\tсколько дней до Нового года (включая сегодняшний): " + ny.GetDay());
        }
        {   // 6) ближайшая пятница 13го числа?
            DateTime fr = new DateTime(year, month, day, 11, 58, 0);
            boolean found = false;
            int counter = 0;
            do {
                if(fr.GetDayOfWeek() == 5 && fr.GetDay() == 13) {
                    break;
                }
                fr.AddDays(1);
                counter++;
                if(counter == 1000) {
                    System.out.println("\too long");
                    break;
                }
            } while(true);
            System.out.println("\n1\tdt " + fr.GetYear() + "-" + fr.GetMonth() + "-" + fr.GetDay() + " week day = " + fr.GetDayOfWeek());
        }
    }

}

/* Выполнить операции с календарём:
 * 1) какой день недели будет через 1024 дня?
 * 2) сколько лет, месяцев и дней назад был День победы 9 мая 1945 года?
 * 3) в какой день Вы родились?
 * 4) какой месяц будет через 17 недель?
 * 5) сколько дней до Нового года?
 * 6) ближайшая пятница 13го числа?
 */
