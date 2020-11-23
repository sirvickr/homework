using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab7
{
    class Program
    {
        // метод , находящий максимальное значение из двух чисел
        static int Max(int x, int y)
        {
            if (x <= y)
                return y;
            return x;
        }

        // метод, вычисляющий значение (x в степени n)/(n+x). С его помощью вычислить выражение
        static double Proportion(double x, double n)
        {
            return Math.Pow(x, n) / (x + n);
        }

        // метод, который вычисляет значения x=sin2(a) и y=cos2(a). 
        static double[] SinCos(double a)
        {
            double[] result = new double[2];
            result[0] = Math.Sin(a);
            result[1] = Math.Cos(a);
            return result;
            //return new double[2]{ Math.Sin(a), Math.Cos(a) };
        }

        // Печать двумерного ступенчатого массива
        static void PrintJagged(int[][] jagged, int n, string caption = "")
        {
            if(caption.Length > 0)
                Console.WriteLine(caption);
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    Console.Write("\t{0}", jagged[i][j]);
                }
                Console.WriteLine("");
            }
        }

        // Печать одномерного массива
        static void PrintVector(int[] vector, int n, string caption = "")
        {
            if (caption.Length > 0)
                Console.Write(caption);
            for (int i = 0; i < n; i++)
            {
                Console.Write("\t{0}", vector[i]);
            }
            Console.WriteLine("");
        }

        // 2. Четные столбцы таблицы заменить на вектор Х.
        static void ReplaceEvenColumns(int[][] jagged, int n, int[] x)
        {
            for (int j = 0; j < n; j++)
            {
                if(j % 2 == 0)
                {
                    for (int i = 0; i < n; i++)
                    {
                        jagged[i][j] = x[i];
                    }
                }
                Console.WriteLine("");
            }
        }

        // Для каждой строки подсчитать количество положительных элементов 
        // и записать данные в новый массив.
        static int[] CountPositive(int[][] jagged, int n)
        {
            int[] result = new int[n];
            for (int i = 0; i < n; i++)
            {
                int count = 0;
                for (int j = 0; j < n; j++)
                {
                    if(jagged[i][j] > 0)
                    {
                        count++;
                    }
                }
                result[i] = count;
                Console.WriteLine("");
            }
            return result;
        }

        // Для каждого столбца найти номер последнего нечетного элемента 
        // и записать данные в новый массив
        static int[] FindLastOdd(int[][] jagged, int n)
        {
            int[] result = new int[n];
            for (int i = 0; i < n; i++)
            {
                // значение по умолчанию предполагает, что в строке нечётных элементов нет
                result[i] = -1; 
            }
            for (int j = 0; j < n; j++)
            {
                for (int i = n - 1; i >= 0; i--)
                {
                    if (jagged[i][j] % 2 == 1)
                    {
                        result[j] = i;
                        break;
                    }
                }
                Console.WriteLine("");
            }
            return result;
        }

        static void Main(string[] args)
        {
            Random rnd = new Random();
            Console.WriteLine("Часть 1");

            string str = "Привет мир";
            char ch = 'и';
            int count = str.WordCount(ch);
            Console.WriteLine("Количество вхождений символа '{0}' в строке \"{1}\": {2}", ch, str, count);

            int a = rnd.Next(-5, 6);
            int b = rnd.Next(-5, 6);
            int c = rnd.Next(-5, 6);
            int d = rnd.Next(-5, 6);
            int m = Max(a, Max(b, Max(c, d)));
            Console.WriteLine("Максимум из {0}, {1}, {2}, {3}: {4}", a, b, c, d, m);

            double x = rnd.Next(1, 5);
            double expression = 0.0;
            for(int i = 0; i < 10; i++)
            {
                expression += Proportion(x, i);
            }
            Console.WriteLine("Результат вычисления выражения для x = {0}: {1}", x, expression);

            Console.WriteLine("Таблица sin и cos от -PI до PI");
            x = -Math.PI;
            double step = Math.PI / 4.0;
            while (x <= Math.PI)
            {
                double[] sincos = SinCos(x);
                Console.WriteLine("{0}\t{1}", sincos[0], sincos[1]);
                x += step;
            }

            Console.WriteLine("\nЧасть 2");
            int n = 5;
            int[][] jagged = new int[n][];
            for (int i = 0; i < n; i++)
            {
                jagged[i] = new int[n];
                for (int j = 0; j < n; j++)
                {
                    jagged[i][j] = i * 10 + j;
                }
            }
            PrintJagged(jagged, n, "Ступенчатый массив:");
            ReplaceEvenColumns(jagged, n, new int[] { 0, -1, 0, 0, -2 });
            PrintJagged(jagged, n, "После замены чётных столбцов:");
            int[] posCounts = CountPositive(jagged, n);
            PrintVector(posCounts, n, "Количество положительных элементов в строках:");
            int[] oddIndices = FindLastOdd(jagged, n);
            PrintVector(oddIndices, n, "Номера последнего нечётного элемента в столбцах:");
            Console.WriteLine("Нажмите любую клавишу...");
            Console.ReadKey();
        }
    }
}
