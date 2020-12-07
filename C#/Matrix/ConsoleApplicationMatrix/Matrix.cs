using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApplicationMatrix
{
    public class MyException : Exception
    {
        public MyException(string s) : base(s)
        {
        }
    }

    public class Matrix
    {
        int[,] m; // Свойство для работы с числом строк.
        public int I { get; set; } // Свойство для работы с числом столбцов. 
        public int J { get; set; } // Конструктор. 

        // Конструктор (Число строк i и столбцов j)
        public Matrix(int i, int j)
        {
            if (i <= 0)
                throw new MyException(string.Format("недопустимое значение i = {0}", i));
            if (j <= 0)
                throw new MyException(string.Format("недопустимое значение j = {0}", j));
            I = i;
            J = j;
            m = new int[i, j];
        }

        // Оператор сложения
        public static Matrix operator +(Matrix a, Matrix b)
        {
            if (a.I != b.I || a.J != b.J)
                throw new MyException(string.Format("размерности не совпадают: {0}x{1} != {2}x{3}", a.I, a.J, b.I, b.J));
            Matrix c = new Matrix(a.I, a.J);
            for (int i = 0; i < a.I; i++)
                for (int j = 0; j < a.J; j++)
                {
                    c[i, j] = a.m[i, j] + b.m[i, j];
                }
            return c;
        }

        // Оператор вычитания
        public static Matrix operator -(Matrix a, Matrix b)
        {
            if (a.I != b.I || a.J != b.J)
                throw new MyException(string.Format("размерности не совпадают: {0}x{1} != {2}x{3}", a.I, a.J, b.I, b.J));
            Matrix c = new Matrix(a.I, a.J);
            for (int i = 0; i < a.I; i++)
                for (int j = 0; j < a.J; j++)
                {
                    c[i, j] = a.m[i, j] - b.m[i, j];
                }
            return c;
        }

        // Оператор умножения
        public static Matrix operator *(Matrix a, Matrix b)
        {
            if (a.J != b.I)
                throw new MyException(string.Format("размерности не согласованы: {0}x{1} и {2}x{3}", a.I, a.J, b.I, b.J));
            Matrix c = new Matrix(a.I, b.J);
            for (int i = 0; i < a.I; i++)
            {
                for (int j = 0; j < b.J; j++)
                {
                    int value = 0;
                    for (int k = 0; k < a.J; k++)
                    {
                        value += a.m[i, k] * b.m[k, j];
                    }
                    c[i, j] = value;
                }
            }
            return c;
        }

        public override bool Equals(object obj)
        {
            return (this as Matrix) == (obj as Matrix);
        }

        // Оператор равенства
        public static bool operator ==(Matrix a, Matrix b)
        {
            if (a.I != b.I || a.J != b.J)
                throw new MyException(string.Format("размерности не совпадают: {0}x{1} != {2}x{3}", a.I, a.J, b.I, b.J));
            bool q = true;
            for (int i = 0; i < a.I; i++)
                for (int j = 0; j < a.J; j++)
                {
                    if (a[i, j] != b[i, j])
                    {
                        q = false;
                        break;
                    }
                }
            return q;
        }

        // Оператор неравенства
        public static bool operator !=(Matrix a, Matrix b)
        {
            return !(a == b);
        }

        // Транспонировать копию матрицы
        public Matrix Transp()
        {
            if (I != J)
                throw new MyException(string.Format("невозможно транспонировать не квадратную матрицу: {0}x{1}", this.I, this.J));
            Matrix c = new Matrix(I, J);
            for (int i = 0; i < I; i++)
                for (int j = 0; j < J; j++)
                {
                    c[i, j] = this[j, i];
                }
            return c;
        }

        // Минимальный элемент
        public int Min()
        {
            int iMin = 0;
            int jMin = 0;
            for (int i = 0; i < I; i++)
                for (int j = 0; j < J; j++)
                {
                    if (this[i, j] < this[iMin, jMin])
                    {
                        iMin = i;
                        jMin = j;
                    }
                }
            return this[iMin, jMin];
        }

        // Отображение элементов в виде строки
        public override string ToString()
        {
            string s = "{";
            for (int i = 0; i < I; i++)
            {
                if (i > 0)
                    s += ",";
                s += "{";
                for (int j = 0; j < J; j++)
                {
                    if (j > 0)
                        s += ",";
                    s += string.Format("{0}", this[i, j]);
                }
                s += "}";
            }
            s += "}";
            return s;
        }

        // Индексатор для доступа к значениям компонентов матрицы. 
        public int this[int i, int j]
        {
            get // Взять элемент с индексами i, j
            {
                if (i < 0 | i > I - 1)
                    throw new MyException(string.Format("неверное значение i = {0}", i));
                if (j < 0 | j > J - 1)
                    throw new MyException(string.Format("неверное значение j = {0}", j));
                return m[i, j];
            }
            set // Записать элемент с индексами i, j
            {
                if (i < 0 | i > I - 1)
                    throw new MyException(string.Format("неверное значение i = {0}", i));
                if (j < 0 | j > J - 1)
                    throw new MyException(string.Format("неверное значение j = {0}", j));
                m[i, j] = value;
            }
        }
        
        //Вывод значений компонентов на консоль. 
        public void Show()
        {
            for (int i = 0; i < I; i++)
            {
                for (int j = 0; j < J; j++)
                {
                    Console.Write("\t" + this[i,j] );
                }
                Console.WriteLine();
            }
            Console.WriteLine();
        }

    }
}
