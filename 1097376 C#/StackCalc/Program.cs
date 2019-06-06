using System;
using System.IO;

namespace StackCalc
{

    // Основной класс приложения
    class Program
    {

        // Ссылка на класс калькулятора
        static Calc calc = new Calc();

        // Основная функця (точка входа) приложения
        static void Main(string[] args)
        {
            Console.WriteLine("Программа-калькулятор");
            try
            {
                // Проверяем наличие аргументов командной строки
                if (args.Length > 0)
                {
                    // Первый аргумент считаем именем файла,
                    // считываем команды из него
                    string fileName = args[0];
                    Console.WriteLine("Ввод из файла \"" + fileName + "\"");
                    // Открываем файл на чтение
                    StreamReader reader = new StreamReader(fileName);
                    // Читаем строки до конца файла
                    while (true)
                    {
                        string cmd = reader.ReadLine();
                        if (cmd != null)
                        {
                            // Каждую прочитанную строку печатаем
                            // и отправляем в калькулятор
                            if (processCommand(cmd, true))
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    // Закрываем файл
                    reader.Close();
                }
                else
                {
                    // Программа запущена без параментров,
                    // считываем команды из консоли
                    Console.WriteLine("Ввод с клавиатуры");
                    while (true)
                    {
                        // Каждую прочитанную строку передаём
                        // калькулятору без вывода на экран
                        if (processCommand(Console.ReadLine(), false))
                        {
                            break;
                        }
                    }
                }
            }
            catch (System.Exception exception)
            {
                Console.WriteLine("Ошибка: "+ exception.Message);
            }
        }

        // Функция обработки отдельной команды
        // Возвражаемое значение:
        // false - продолжаем ввод команд
        // true - завершаем работу
        static bool processCommand(string cmd, bool trace)
        {
            // Комментарии и пустые строки пропускаем
            if (cmd.Length == 0 || cmd[0] == '#')
            {
                return false;
            }

            // Выводим исходную команду на консоль
            // (при вводе с клавиатуры не имеет смысла,
            // т.к. пользователь уже её сам набрал в консоли)
            if (trace)
            {
                Console.WriteLine(cmd);
            }

            // Переводим все символы в верхний регистр
            // (убираем зависимость от регистра)
            cmd = cmd.ToUpper();

            // Разбиваем команду на имя команды 
            // и аргументы (если есть)
            string[] tokens = cmd.Split(new char[] { ' ' });

            // Первый элемент обязан содержать имя команды
            if (tokens.Length == 0)
            {
                return false;
            }

            if (tokens[0] == "DEFINE")
            {
                // Задать значение параметра
                // (ожидаем: DEFINE имя значение)
                if (tokens.Length == 3)
                {
                    try
                    {
                        calc.Define(tokens[1], tokens[2]);
                    }
                    catch (System.FormatException exeption)
                    {
                        Console.WriteLine("Formatxception: " + 
                            exeption.Message);
                    }
                }
            }
            else if (tokens[0] == "PUSH")
            {
                // Положить число на стек
                // (ожидаем: PUSH значение)
                if (tokens.Length == 2)
                {
                    calc.Push(tokens[1]);
                }
            }
            else if (cmd == "POP")
            {
                // Снять число со стека
                calc.Pop();
            }
            else if (cmd == "PRINT")
            {
                // Печатаем верхушку стека
                calc.Print();
            }
            else if (cmd == "+")
            {
                // Складываем два верхних операнда
                calc.Add();
            }
            else if (cmd == "-")
            {
                // Вычитаем два верхних операнда
                calc.Sub();
            }
            else if (cmd == "*")
            {
                // Перемножаем два верхних операнда
                calc.Mul();
            }
            else if (cmd == "/")
            {
                // Делим два верхних операнда
                calc.Div();
            }
            else if (cmd == "SQRT")
            {
                // Извлекаем корень из операнда на верхушине стека
                calc.Sqrt();
            }
            else if (cmd == "END")
            {
                // Возвращаем признак завершения работы
                return true;
            }
            else
            {
                // Сообщаем об ошибке, но не прерываем работу
                Console.WriteLine("Неизвестная команда: \"" + cmd + "\"");
            }
            // Возвращаем признак продолжения работы
            return false;
        }

    }

}
