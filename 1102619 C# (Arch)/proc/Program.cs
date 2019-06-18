using System;
using System.IO;

namespace proc
{
    class Program
    {
        // Ссылка на класс вычислительной машины
        static Computer computer = new Computer();

        // Основная функция (точка входа) приложения
        static void Main(string[] args)
        {
            // Загрузка программы
            try
            {
                // будем записывать команды в начало памяти
                computer.prepareLoad();
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
                            if (computer.addCommand(cmd, true))
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
                        if (computer.addCommand(Console.ReadLine(), false))
                        {
                            break;
                        }
                    }
                }
            }
            catch (System.Exception exception)
            {
                Console.WriteLine("Ошибка: " + exception.Message);
            }


            computer.runProgram();
        }
    }
}
