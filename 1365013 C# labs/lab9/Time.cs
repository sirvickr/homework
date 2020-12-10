using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab9
{
    public class Time
    {
        // Конструктор

        public Time(int hour = 0, int min = 0, int sec = 0)
        {
            int totalSec = sec;
            Sec = totalSec % 60;
            totalSec -= Sec;

            int totalMin = min + totalSec / 60;
            Min = totalMin % 60;
            totalMin -= Min;

            Hour = hour + totalMin / 60;
        }

        // Публичные свойства

        public int Hour
        {
            set
            {
                hours = value;
            }
            get
            {
                return hours;
            }
        }

        public int Min
        {
            set
            {
                minutes = value;
            }
            get
            {
                return minutes;
            }
        }

        public int Sec
        {
            set
            {
                seconds = value;
            }
            get
            {
                return seconds;
            }
        }

        // Строковое представление времени

        public override string ToString()
        {
            return $"{Hour}:{Min}:{Sec}";
        }

        // Оператор преобразования из строки

        public static implicit operator Time(string s)
        {
            string[] tokens = s.Split(':');
            if (tokens.Length != 3)
                return new Time();
            return new Time(Convert.ToInt32(tokens[0]), Convert.ToInt32(tokens[1]), Convert.ToInt32(tokens[2]));
        }

        // Операторы преобразования времени в секунды и обратно

        public static implicit operator Time(int sec)
        {
            return new Time(0, 0, sec);
        }

        public static explicit operator int(Time time)
        {
            return time.Sec + time.Min * 60 + time.Hour * 3600;
        }

        // Внутренние данные класса

        private int hours;
        private int minutes;
        private int seconds;
    }
}
