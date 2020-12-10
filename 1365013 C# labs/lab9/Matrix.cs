using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab9
{
    public class Matrix
    {
        // Конструктор с параметрами по умолчанию

        public Matrix(int m = 1, int n = 1, short init = 0)
        {
            Rows = m;
            Cols = n;
            data = new short[Rows, Cols];
            for(int i = 0; i < Rows; i++)
            {
                for (int j = 0; j < Rows; j++)
                {
                    data[i, j] = init;
                }
            }
        }

        // Конструктор копирования

        public Matrix(Matrix other) : this(other.Rows, other.Cols, Convert.ToInt16(other[0, 0] + 1))
        {
        }

        // Деструктор

        ~Matrix()
        {
        }

        // Индексатор

        public short this[int i, int j]
        {
            get
            {
                return data[i, j];
            }
            set
            {
                data[i, j] = value;
            }
        }

        // Операторы преобразования типа

        public static implicit operator Matrix(short x)
        {
            return new Matrix ( 1, 1, x );
        }

        public static explicit operator double(Matrix matrix)
        {
            double sum = 0.0;
            int count = matrix.Rows * matrix.Cols;
            for (int i = 0; i < matrix.Rows; i++)
            {
                for (int j = 0; j < matrix.Rows; j++)
                {
                    sum += matrix.data[i, j];
                }
            }
            return sum / count;
        }

        // Вывод на печать

        public void Print(string prefix = null)
        {
            if(prefix != null)
            {
                Console.WriteLine(prefix);
            }
            for (int i = 0; i < Rows; i++)
            {
                for (int j = 0; j < Rows; j++)
                {
                    Console.Write($" {data[i, j]}");
                }
                Console.WriteLine("");
            }
        }

        public int Rows { get; private set; }
        public int Cols { get; private set; }

        private readonly short[,] data = null;
    }
}
