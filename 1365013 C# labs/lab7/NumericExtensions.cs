using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab7
{
    public static class NumericExtensions
    {
        public static int DividersCount(this int num)
        {
            int result = 1;
            for (int i = 2; i <= Math.Abs(num); i++)
                if (num % i == 0)
                    result++;
            return result;
        }

        public static double Fraction(this double d)
        {
            double d3 = Math.Truncate(d);
            double result = d - d3;
            return result;
        }

        public static void NullifyNegative(this int[] array)
        {
            Console.Write("array:");
            for (int i = 0; i < array.Length; i++)
            {
                if(array[i] < 0)
                {
                    array[i] = 0;
                }
                Console.Write(" {0}", array[i]);
            }
            Console.WriteLine("");
        }

        public static void Print(this int[] array, string prefix = "")
        {
            Console.Write(prefix);
            for (int i = 0; i < array.Length; i++)
                Console.Write(" {0}", array[i]);
            Console.WriteLine("");
        }
    }
}
