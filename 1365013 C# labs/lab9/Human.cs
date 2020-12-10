using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab9
{
    // Класс, представляющий млекопитающее (9.3.1)
    public class Mammal
    {
        public enum Genders { Male, Female };

        // Конструктор с параметрами (9.3.2)

        public Mammal(Genders gender, int age = 0)
        {
            Gender = gender;
            Age = age;
            Console.Write("Mammal - конструктор по умолчанию: ");
            Print();
        }

        // Деструктор (9.3.5)
        ~Mammal()
        {
            Console.Write("Mammal - деструктор:");
            Print();
        }

        // Свойства только для чтения (9.4.1)

        public Genders Gender { get; }

        public int Age { get; private set; }

        public readonly int BaseIQ = 1;

        // Методы класса (9.2.2)

        public string GenderRep()
        {
            if (Gender == Genders.Male)
                return "Самец";
            return "Самка";
        }

        public void Grow()
        {
            Age = Age + 1;
        }

        public void Print()
        {
            Console.WriteLine($" Млекопитающее, {Age} лет, {GenderRep()}");
        }

        protected bool disposed = false;
    }

    // Класс, представляющий человека (9.2.1)
    public class Human : Mammal
    {
        // Сокрытие членов базового класса (9.4.2)
        public new readonly int BaseIQ = 60;

        // Конструкторы (9.3.2)

        public Human() : this(Genders.Male, "Новый человек 1")
        {
            population++;
            Console.Write("Human - конструктор по умолчанию: ");
            Print();
        }

        public Human(Genders gender, String name, int age = 0) : base(gender, age)
        {
            population++;
            Name = name;
            Console.Write("Human - конструктор с параметрами: ");
            Print();
        }

        // Деструктор (9.3.5)
        ~Human()
        {
            population--;
            Console.Write("Human - деструктор: ");
            Print();
        }

        // Свойство для чтения и записи (9.4.1)
        public string Name { get; set; }

        // Свойство для записи (9.4.1)
        private decimal balance;
        public decimal Balance
        {
            set
            {
                balance = value;
            }
        }

        // Методы класса (9.2.2)
        void Say(string sth)
        {
            Console.WriteLine(sth);
        }

        // Сокрытие членов базового класса (9.4.2)
        public new string GenderRep()
        {
            if (Gender == Genders.Male)
                return "мужчина";
            return "женщина";
        }

        public new void Print()
        {
            Console.WriteLine($" Человек по имени {Name}, {Age} лет, {GenderRep()}");
        }

        // Статическое поле - объём населения (9.3.3)
        static int population = 0;
    }

    public class AccountRecord<T>
    {
        public T Id { get; set; }
        public double Sum { get; set; } = 0.0;
    }

    // Класс, представляющий служащего (9.3.1)
    public class Employee : Human
    {
        // Конструкторы (9.3.2)

        public Employee() : base()
        {
            Console.Write("Employee - конструктор по умолчанию: ");
            Print();
        }

        public Employee(Genders gender, String name, String company, int age = 0, double salary = 0.0) 
            : base(gender, name, age)
        {
            Company = company;
            Salary = salary;
            account = new AccountRecord<int> { Id = 100500 };
            Console.Write("Employee - конструктор по умолчанию: ");
            Print();
        }

        // Деструктор (9.3.5)
        ~Employee()
        {
            Console.Write("Employee - деструктор: ");
            Print();
        }

        // Свойства для чтения и записи (9.4.1)

        public string Company { get; set; }

        public double Salary { get; set; }

        // Сокрытие членов базового класса (9.4.2)

        public new void Print()
        {
            Console.WriteLine($" Сотрудник {Company}, {Age} лет, {GenderRep()}");
        }

        // поле - экземпляр параметризованного класса (9.3.4)
        private readonly AccountRecord<int> account = null;
    }

}
