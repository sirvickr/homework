using System;
using System.Text.RegularExpressions;

namespace register
{
    [Serializable]
    public class Doctor: ICloneable
    {
        private DateTime _birthDate;
        private int _cabinetNumber;
        private string _name;
        private string _specialization;
        public Doctor()
        {
            Timetable = new Timetable();
        }
        public Doctor(string name,
        DateTime birthDate,
        string speciality,
        int cabinetNumber,
        Timetable timetable)
        : this()
        {
            Name = name;
            BirthDate = birthDate;
            Specialization = speciality;
            CabinetNumber = cabinetNumber;
            Timetable = timetable;
        }
        public Timetable Timetable { get; private set; }
        public string Name
        {
            get { return _name; }
            set
            {
                value = value.Trim();
                //if (!Regex.IsMatch(value, @ "^[А - я] +\s + ([А - я]\.\s *) { 2}$"))
                    //throw new ArgumentException("Ф.И.О.врача задано неверно");
                _name = value;
            }
        }
        public DateTime BirthDate
        {
            get { return _birthDate; }
            set
            {
                //if ((DateTime.Today – value).TotalDays < 6564) // 18 лет
                    //throw new ArgumentException("Дата рождения задана неверно");
                _birthDate = value;
            }
        }
        public string Specialization
        {
            get { return _specialization; }
            set
            {
                value = value.Trim();
                //if (!Regex.IsMatch(value, @ "^[А - я] +$"))
                    //throw new ArgumentException("Специальность задана неверно");
                _specialization = value;
            }
        }
        public int CabinetNumber
        {
            get { return _cabinetNumber; }
            set
            {
                if (value <= 0)
                    throw new ArgumentException("Номер кабинета должен быть больше ноля");
                _cabinetNumber = value;
            }
        }
        // IClonable
        public object Clone()
        {
            return this.MemberwiseClone();
        }
    }
}
