using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.Serialization;

namespace register
{
    [Serializable]
    public class DayShedule : Dictionary<TimeSpan, string>
    {
        public DayOfWeek DayOfWeek { get; private set; }
        public DateTime Date { get; private set; }
        public TimeSpan StartTime { get; private set; }
        public TimeSpan EndTime { get; private set; }
        public TimeSpan Duration { get; private set; }
        public TimeSpan Interval { get; private set; }
        public bool IsSet { get; private set; }
        public bool IsExpired
        {
            get { return DateTime.Today > Date; }
        }
        protected DayShedule(SerializationInfo info, StreamingContext context)
        : base(info, context)
        {
            DayOfWeek = (DayOfWeek)info.GetValue("DayOfWeek", typeof(DayOfWeek));
            IsSet = info.GetBoolean("IsSet");
            if (!IsSet)
                return;
            Date = info.GetDateTime("Date");
            StartTime = (TimeSpan)info.GetValue("StartTime", typeof(TimeSpan));
            EndTime = (TimeSpan)info.GetValue("EndTime", typeof(TimeSpan));
            Duration = (TimeSpan)info.GetValue("Duration", typeof(TimeSpan));
            Interval = (TimeSpan)info.GetValue("Interval", typeof(TimeSpan));
        }
        public DayShedule(DayOfWeek day)
        {
            DayOfWeek = day;
        }
        public void Set(TimeSpan startTime, TimeSpan endTime, TimeSpan interval)
        {
            if (startTime.Days > 0)
                throw new ArgumentException("Начальное время задано неверно");
            if (endTime.Days > 0)
                throw new ArgumentException("Конечное время задано неверно");
            if (startTime >= endTime)
                throw new ArgumentException("Начальное время должно быть больше конечного");
            StartTime = startTime;
            EndTime = endTime;
            Duration = EndTime - StartTime;
            if (interval > Duration)
                throw new ArgumentException("Интервал превышает рабочий промежуток");
            IsSet = true;
            Interval = interval;
            SetNextDateOfDayOfWeek();
            Clear();
            TimeSpan floatBound = StartTime;
            while (floatBound < EndTime)
            {
                Add(floatBound, string.Empty);
                floatBound += interval;
            }
        }
        public void Unset()
        {
            IsSet = false;
        }
        public void SyncDate()
        {
            if (IsExpired)
            {
                double delta = (DateTime.Today - Date).TotalDays / 7;
                var factor = (int)(Math.Abs((int)delta - delta) < double.Epsilon
                ? delta
                : delta + 1);
                Date += new TimeSpan(factor * 7, 0, 0, 0);
                ClearTasks();
            }
        }
        private void ClearTasks()
        {
            foreach (TimeSpan time in Keys.ToList())
                this[time] = string.Empty;
        }
        private void SetNextDateOfDayOfWeek()
        {
            Date = DateTime.Today;
            while (Date.DayOfWeek != DayOfWeek)
                Date = Date.AddDays(1);
        }
        public override void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            base.GetObjectData(info, context);
            info.AddValue("DayOfWeek", DayOfWeek);
            info.AddValue("IsSet", IsSet);
            if (!IsSet)
                return;
            info.AddValue("Date", Date);
            info.AddValue("StartTime", StartTime);
            info.AddValue("EndTime", EndTime);
            info.AddValue("Duration", Duration);
            info.AddValue("Interval", Interval);
        }
    }
}
