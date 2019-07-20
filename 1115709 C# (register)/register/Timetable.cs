using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace register
{
    public class Timetable: IEnumerable<DayShedule>
    {
        public readonly DayShedule Friday;
        public readonly DayShedule Monday;
        public readonly DayShedule Saturday;
        public readonly DayShedule Sunday;
        public readonly DayShedule Tuesday;
        public readonly DayShedule Wednesday;
        public readonly DayShedule Thursday;
        private readonly List<DayShedule> _days;
        public Timetable()
        {
            Monday = new DayShedule(DayOfWeek.Monday);
            Tuesday = new DayShedule(DayOfWeek.Tuesday);
            Wednesday = new DayShedule(DayOfWeek.Wednesday);
            Thursday = new DayShedule(DayOfWeek.Thursday);
            Friday = new DayShedule(DayOfWeek.Friday);
            Saturday = new DayShedule(DayOfWeek.Saturday);
            Sunday = new DayShedule(DayOfWeek.Sunday);
            _days = new List<DayShedule>
            {
                Monday,
                Tuesday,
                Wednesday,
                Thursday,
                Friday,
                Saturday,
                Sunday
            };
        }
        public DayShedule this[int index]
        {
            get { return _days[index]; }
        }
        public void RemoveExpired()
        {
            foreach (DayShedule day in this)
                day.SyncDate();
        }
        // IEnumerable
        public IEnumerator<DayShedule> GetEnumerator()
        {
            return _days.Where(t => t.IsSet).GetEnumerator();
        }
        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}
