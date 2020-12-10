using System;
using Tasks;

namespace lab7
{
    class Program
    {

        static void Main(string[] args)
        {
            Random rnd = new Random();
            Arithmetics arith = new Arithmetics();
            Arrays arrays = new Arrays();

            Console.WriteLine("Часть 1");

            // 2. Написать метод max(x, y), находящий максимальное значение из двух чисел.
            //    С его помощью найти максимальное значение из четырёх чисел a, b, c, d.
            int a = rnd.Next(-5, 6);
            int b = rnd.Next(-5, 6);
            int c = rnd.Next(-5, 6);
            int d = rnd.Next(-5, 6);
            int m = arith.Max(a, arith.Max(b, arith.Max(c, d)));
            Console.WriteLine("\nМаксимум из {0}, {1}, {2}, {3}: {4}", a, b, c, d, m);

            // Определите метод расширения, подсчитывающий количество делителей в исходном целом числе.
            Console.Write("Введите целое число для подсчёта количества делителей: ");
            int num = Convert.ToInt32(Console.ReadLine());
            Console.WriteLine("\nколичество делителей в числе {0}: {1}", num, num.DividersCount());

            // 5. Написать метод, вычисляющий значение xn/(n+x). С его помощью вычислить выражение.
            double x = rnd.Next(1, 5);
            double expression = 0.0;
            for(int i = 0; i < 10; i++)
            {
                expression += arith.Proportion(x, i);
            }
            Console.WriteLine("\nРезультат вычисления выражения для x = {0}: {1}", x, expression);

            // Определите метод расширения, вычисляющий дробную часть в выражении
            Console.WriteLine("\nДробная часть выражения: {0}", expression.Fraction());

            // 11. Написать метод, который вычисляет значения x=sin2(a) и y=cos2(a). 
            //     Напечатать таблицу значений от –π до π с шагом π/4
            Console.WriteLine("\nТаблица sin и cos от -PI до PI");
            x = -Math.PI;
            double step = Math.PI / 4.0;
            while (x <= Math.PI)
            {
                double[] sincos = arith.SinCos(x);
                Console.WriteLine("{0}\t{1}", sincos[0], sincos[1]);
                x += step;
            }

            // Написать метод расширения, обнуляющий в массиве все отрицательные элементы.
            int[] array = new int[] { 1, -1, 2, -2 };
            array.Print("\nМассив до обнуления: ");
            array.NullifyNegative();
            array.Print("Массив после обнуления: ");

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
            arrays.PrintJagged(jagged, n, "\nСтупенчатый массив:");
            arrays.ReplaceEvenColumns(jagged, n, new int[] { 0, -1, 0, 0, -2 });
            arrays.PrintJagged(jagged, n, "\nПосле замены чётных столбцов:");
            int[] posCounts = arrays.CountPositive(jagged, n);
            arrays.PrintVector(posCounts, n, "\nКоличество положительных элементов в строках:");
            int[] oddIndices = arrays.FindLastOdd(jagged, n);
            arrays.PrintVector(oddIndices, n, "\nНомера последнего нечётного элемента в столбцах:");
            Console.WriteLine("\nНажмите любую клавишу...");
            Console.ReadKey();
        }
    }
}
