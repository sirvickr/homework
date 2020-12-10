using System;

namespace Tasks
{
    public class Arithmetics
    {
        // методы, находящие максимальное значение из двух чисел (три варианта вызова)
        public int Max(int x, int y)
        {
            if (x <= y)
                return y;
            return x;
        }

        public float Max(float x, float y)
        {
            if (x <= y)
                return y;
            return x;
        }

        public double Max(double x, double y)
        {
            if (x <= y)
                return y;
            return x;
        }

        // метод, вычисляющий значение (x в степени n)/(n+x). С его помощью вычислить выражение
        public double Proportion(double x, double n = 1.0)
        {
            return Math.Pow(x, n) / (x + n);
        }

        // метод, который вычисляет значения x=sin2(a) и y=cos2(a). 
        public double[] SinCos(double a = Math.PI / 2.0)
        {
            double[] result = new double[2];
            result[0] = Math.Sin(a);
            result[1] = Math.Cos(a);
            return result;
            //return new double[2]{ Math.Sin(a), Math.Cos(a) };
        }
    }
}
