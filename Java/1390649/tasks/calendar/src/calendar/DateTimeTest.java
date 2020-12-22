package calendar;

import static org.junit.Assert.*;

import org.junit.Test;
import java.util.Calendar;
import java.util.GregorianCalendar;

public class DateTimeTest {

	@Test
	public void testWeekDay() {
		// 1) какой день недели будет через 1024 дня?
		DateTime dt = today();
		dt.addDays(1024);
		
		Calendar calendar = new GregorianCalendar();
		calendar.add(Calendar.DAY_OF_YEAR, 1024);
		assertEquals(calendar.get(Calendar.DAY_OF_WEEK) - 2, dt.GetDayOfWeek());
	}
	
	@Test
	public void testVictoryDay() {
		// 2) сколько лет, месяцев и дней назад был День победы 9 мая 1945 года?
		DateTime dt = today();
		DateTime vd = new DateTime(1945, 5, 9, 0, 0, 0);
		dt.sub(vd);
		assertEquals(76, dt.GetYear());
		assertEquals(8, dt.GetMonth());
		assertEquals(15, dt.GetDay());
	}
	
	@Test
	public void testBirthDay() {
		// 3) в какой день Вы родились?
		DateTime bithday = new DateTime(2001, 10, 18, 0, 0, 0);
		assertEquals(4, bithday.GetDayOfWeek());
	}
	
	@Test
	public void testAddWeeks() {
		// 4) какой месяц будет через 17 недель?
		DateTime dt = today();
		dt.addWeeks(17);
		assertEquals(4, dt.GetMonth());
	}
	
	@Test
	public void testNewYear() {
		// 5) сколько дней до Нового года?
		DateTime dt = today();
		DateTime ny = new DateTime(dt.GetYear(), 12, 31, 23, 59, 59);
		ny.sub(dt);
		assertEquals(11, ny.GetDay());
	}
	
	@Test
	public void testFriday13() {
		// 6) ближайшая пятница 13го числа?
		DateTime fr = today();
		do {
			if(fr.GetDayOfWeek() == 5 && fr.GetDay() == 13) {
				break;
			}
			fr.addDays(1);
		} while(true);
		assertEquals(2021, fr.GetYear());
		assertEquals(8, fr.GetMonth());
		assertEquals(13, fr.GetDay());
		assertEquals(5, fr.GetDayOfWeek());
	}

	private DateTime today() {
		Calendar calendar = new GregorianCalendar();
		calendar.add(Calendar.DAY_OF_MONTH, -1);
		return new DateTime(calendar.get(Calendar.YEAR), 
				calendar.get(Calendar.MONTH) + 1, 
				calendar.get(Calendar.DAY_OF_MONTH), 0, 0, 1);
	}

}
