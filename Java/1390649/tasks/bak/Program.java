/* Реализовать класс григорианского календаря со следующими особенностями поведения:
 * 1) каждый четвёртый год високосный
 * 2) каждый завершающий год века, если он не делится на 400, не является високосным
 * 3) для заданной даты определить день недели
 * 4) поддержка арифметических операций сложение и вычитание (дата +- дата, месяц, год)
*/

package calendar;

import java.util.Date;

import static org.junit.Assert.assertEquals;

import java.util.Calendar;
import java.util.GregorianCalendar;

public class Program {

	public static void main(String[] args) {
		int year = 2020;
		int month = 12;
		int day = 21;
		
		{
			//Calendar calendar = new GregorianCalendar(year, Calendar.DECEMBER, day);
			Calendar calendar = new GregorianCalendar();
			
			calendar.add(Calendar.DAY_OF_MONTH, -1);
			year = calendar.get(Calendar.YEAR);
			month = calendar.get(Calendar.MONTH) + 1;
			day = calendar.get(Calendar.DAY_OF_MONTH);
			System.out.println("\n1\tdt " + year + "-" + month + "-" + day);
			Date date = calendar.getTime();
			System.out.println("\t" + date);
		}
		{
			// 1) какой день недели будет через 1024 дня?
			DateTime dt = new DateTime(year, month, day, 11, 58, 0);
			dt.addDays(1024);
			System.out.println("\n1\tdt " + dt.GetYear() + "-" + dt.GetMonth() + "-" + dt.GetDay() + " " + dt.GetHour() + ":" + dt.GetMinute() + ":" + dt.GetSecond() + " week day = " + dt.GetDayOfWeek());
			
			Calendar calendar = new GregorianCalendar();
			calendar.add(Calendar.DAY_OF_YEAR, 1024);
			//assertEquals(calendar.get(Calendar.DAY_OF_WEEK) - 2, dt.GetDayOfWeek());
			System.out.println("1\tday of week " + (calendar.get(Calendar.DAY_OF_WEEK) - 1) + "\t" + calendar.getTime());
		}
		{
			// сколько лет, месяцев и дней назад был День победы 9 мая 1945 года?
			DateTime dt = new DateTime(year, month, day, 0, 0, 0);
			DateTime vd = new DateTime(1945, 5, 9, 0, 0, 0);
			dt.sub(vd);
			System.out.println("\n2\t" + dt.GetYear() + " years " + dt.GetMonth() + " months " + dt.GetDay() + " days");
		}
		{
			// в какой день Вы родились?
			DateTime bithday = new DateTime(2001, 10, 18, 0, 0, 0);
			System.out.println("\n3\tв какой день Вы родились: " + bithday.GetDayOfWeek());
		}
		{
			// 4) какой месяц будет через 17 недель?
			DateTime dt = new DateTime(year, month, day, 11, 58, 0);
			dt.addWeeks(17);
			System.out.println("\n4\tкакой месяц будет через 17 недель: " + dt.GetMonth());
		}
		{
			// 5) сколько дней до Нового года?
			DateTime dt = new DateTime(year, month, day, 11, 58, 0);
			DateTime ny = new DateTime(year, 12, 31, 23, 59, 59);
			ny.sub(dt);
			System.out.println("\n5\tсколько дней до Нового года (включая сегодняшний): " + ny.GetDay());
		}
		DateTime fr = new DateTime(year, month, day, 11, 58, 0);
		do {
			if(fr.GetDayOfWeek() == 5 && fr.GetDay() == 13) {
				break;
			}
			fr.addDays(1);
		} while(true);
		System.out.println("\n6\tdt " + fr.GetYear() + "-" + fr.GetMonth() + "-" + fr.GetDay() + " week day = " + fr.GetDayOfWeek());
	}

}
