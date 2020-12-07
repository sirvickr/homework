using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApplicationMatrix
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                //Создаём матрицу a. 
                Matrix a = new Matrix(3, 3);
                //Создаём матрицу b. 
                Matrix b = new Matrix(3, 3);
                //Объявляем матрицу c. 
                Matrix c;
                //Заполняем матрицу a. 
                for (int i = 0; i < a.I; i++)
                {
                    for (int j = 0; j < a.J; j++)
                    {
                        a[i, j] = a.J * i + j;
                    }
                }
                //Выводим матрицу a. 
                a.Show();
                //Заполняем матрицу b. 
                for (int i = 0; i < a.I; i++)
                {
                    for (int j = 0; j < a.J; j++)
                    {
                        b[i, j] = a.J * i + j + 1;
                    }
                }

                //Выводим матрицу a. 
                b.Show();

                //Складываем матрицы a и b. 
                c = a + b;

                //Выводим матрицу c. 
                c.Show();
            }
            catch (MyException e)
            {
                Console.WriteLine(e.Message);
            }

        }
    }
}
