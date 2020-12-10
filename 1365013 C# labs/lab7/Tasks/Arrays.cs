using System;

namespace Tasks
{
    public class Arrays
    {
        // Печать двумерного ступенчатого массива
        public void PrintJagged(int[][] jagged, int n, string caption = "")
        {
            if (caption.Length > 0)
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
        public void PrintVector(int[] vector, int n, string caption = "")
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
        public void ReplaceEvenColumns(int[][] jagged, int n, int[] x)
        {
            for (int j = 0; j < n; j++)
            {
                if (j % 2 == 0)
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
        public int[] CountPositive(int[][] jagged, int n)
        {
            int[] result = new int[n];
            for (int i = 0; i < n; i++)
            {
                int count = 0;
                for (int j = 0; j < n; j++)
                {
                    if (jagged[i][j] > 0)
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
        public int[] FindLastOdd(int[][] jagged, int n)
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
    }
}
