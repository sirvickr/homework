using System;
using System.Collections.Generic;
using System.Globalization;

namespace StackCalc
{
    
    /*
     * Класс, отвечающий за основные функции программы - 
     * управление стеком, хранение параментров и выполнение операций
     */

    class Calc
    {

        Stack<string> stack = new Stack<string>();
        Dictionary<string, double> defs = new Dictionary<string, double>();

        // Задать значение сохраняемого параметра
        // параметры функции:
        // name - имя сохраняемого параметра
        // str - строковое значение сохраняемого параметра
        public void Define(string name, string str)
        {
            // Преобразовать строковое значение 
            // в число с плавающей точкой
            double value = double.Parse(str.Trim(),
                CultureInfo.InvariantCulture.NumberFormat);
            if (defs.ContainsKey(name))
            {
                // Заменить существующее значение
                defs[name] = value;
            }
            else
            {
                // Добавить новое определение
                defs.Add(name, value);
            }
        }

        // Поместить операнд на вершину стека
        // параметры функции:
        // arg - строковое представление операнда
        public void Push(string arg)
        {
            stack.Push(arg);
        }

        // Снять операнд с вершины стека и напечать
        public void Pop()
        {
            if (stack.Count > 0)
            {
                Console.WriteLine(GetValue(stack.Pop()).ToString());
            }
            else
            {
                Console.WriteLine("Стек пуст");
            }
        }

        // Напечатать значение операнда на вершине стека
        // если на вершине стека находится имя сохраённого
        // параметра, то его значение извлекается из хранилища
        public void Print()
        {
            if (stack.Count > 0)
            {
                Console.WriteLine(GetValue(stack.Peek()).ToString());
            }
            else
            {
                Console.WriteLine("Стек пуст");
            }
        }

        // Складывает два верхних операнда,
        // результат помещает обратно в стек
        public void Add()
        {
            if (stack.Count > 1)
            {
                // Забрать из стека первый операнд
                double a = GetValue(stack.Pop());
                // Забрать из стека второй операнд
                double b = GetValue(stack.Pop());
                // Выполнить операцию (сложение)
                double c = a + b;
                // Поместить результат обратно в стек
                stack.Push(c.ToString());
            }
            else
            {
                Console.WriteLine("Недостаточно аргументов для операции");
            }
        }

        // Вычитает два верхних операнда,
        // результат помещает обратно в стек
        public void Sub()
        {
            if (stack.Count > 1)
            {
                // Забрать из стека первый операнд
                double a = GetValue(stack.Pop());
                // Забрать из стека второй операнд
                double b = GetValue(stack.Pop());
                // Выполнить операцию (вычитание)
                double c = a - b;
                // Поместить результат обратно в стек
                stack.Push(c.ToString());
            }
            else
            {
                Console.WriteLine("Недостаточно аргументов для операции");
            }
        }

        // Перемножает два верхних операнда,
        // результат помещает обратно в стек
        public void Mul()
        {
            if (stack.Count > 1)
            {
                // Забрать из стека первый операнд
                double a = GetValue(stack.Pop());
                // Забрать из стека второй операнд
                double b = GetValue(stack.Pop());
                // Выполнить операцию (умножение)
                double c = a * b;
                // Поместить результат обратно в стек
                stack.Push(c.ToString());
            }
            else
            {
                Console.WriteLine("Недостаточно аргументов для операции");
            }
        }

        // Делит два верхних операнда,
        // результат помещает обратно в стек
        public void Div()
        {
            if (stack.Count > 1)
            {
                // Забрать из стека первый операнд
                double a = GetValue(stack.Pop());
                // Забрать из стека второй операнд
                double b = GetValue(stack.Pop());
                // Выполнить операцию (деление)
                double c = a / b;
                // Поместить результат обратно в стек
                stack.Push(c.ToString());
            }
            else
            {
                Console.WriteLine("Недостаточно аргументов для операции");
            }
        }

        // Извлекает корень из операнда на верхушине стека,
        // результат помещает обратно в стек
        public void Sqrt()
        {
            if (stack.Count > 0)
            {
                // Забрать из стека аргумент
                double x = GetValue(stack.Pop());
                if(x >= 0)
                {
                    // Выполнить операцию (взятие квадратного корня)
                    x = Math.Sqrt(x);
                }
                else
                {
                    Console.WriteLine("Невозможно извлечь квадратный корень из отрицательного числа");
                }
                // Поместить результат обратно в стек
                stack.Push(x.ToString());
            }
            else
            {
                Console.WriteLine("Недостаточно аргументов для операции");
            }
        }

        // Возвращает действительное число, соответствующее
        // переданному строковому аргументу.
        // Если это число, то функция преобразует строковое
        // представление в число с плавающей точкой, иначе
        // извлекает число из хранилища именованных параметров,
        // параметры функции:
        // arg - строковое представление операнда
        private double GetValue(string arg)
        {
            double value = 0.0;
            try
            {
                // Пробуем преобразовать строковый аргумент в число с плавающей точкой
                value = double.Parse(arg.Trim(),
                    CultureInfo.InvariantCulture.NumberFormat);
            }
            catch (System.FormatException)
            {
                // Ошибка - возможно, это имя параметра, 
                // пробуем найти его в коллекции определений
                if (defs.ContainsKey(arg))
                {
                    // Заменить существующее значение
                    value = defs[arg];
                }
                else
                {
                    // Ничего не получилось - выдать ошибку
                    throw new System.Exception("Некорректный аргумент: " + arg);
                }
            }
            return value;
        }

    }

}
