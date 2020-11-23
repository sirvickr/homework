using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace lab10
{
    class Program
    {
        static Random rnd = new Random();

        static void ReverseNaturalSeq(int n, string path)
        {
            // Создать файл и записать в него обратные натуральные числа 1, 1/2, ... , 1/n
            try
            {
                if (File.Exists(path))
                {
                    File.Delete(path);
                }

                // запись последовательности
                using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
                {
                    for (int i = 1; i <= n; i++)
                    {
                        double item = 1.0 / Convert.ToDouble(i);
                        writer.Write(item);
                    }
                }
                // чтение последовательности
                using (BinaryReader reader = new BinaryReader(File.Open(path, FileMode.Open)))
                {
                    int i = 1;
                    // пока не достигнут конец файла, считываем каждое значение
                    while (reader.PeekChar() > -1)
                    {
                        double item = reader.ReadDouble();
                        if ((i % 3) == 0)
                        {
                            Console.WriteLine("{0}\t{1}", i, item);
                        }
                        i++;
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        static void IntSeq(int n, string path)
        {
            // Дана последовательность из n целых чисел. 
            // Создать файл и записать в него все отрицательные числа последовательности. 
            // Вывести содержимое файла на экран.
            try
            {
                if (File.Exists(path))
                {
                    File.Delete(path);
                }

                // запись последовательности
                using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
                {
                    Console.Write("\nПоследовательность:");
                    for (int i = 0; i < n; i++)
                    {
                        int item = rnd.Next(-9, 10);
                        Console.Write(" {0}", item);
                        if (item < 0)
                        {
                            writer.Write(item);
                        }
                    }
                    Console.WriteLine("");
                }
                // чтение последовательности
                using (BinaryReader reader = new BinaryReader(File.Open(path, FileMode.Open)))
                {
                    Console.Write("Содержимое файла:");
                    // пока не достигнут конец файла, считываем каждое значение
                    while (reader.PeekChar() > -1)
                    {
                        int item = reader.ReadInt32();
                        Console.Write(" {0}", item);
                    }
                    Console.WriteLine("");
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        static void RealSeq(int n, string path)
        {
            // Дана последовательность из n вещественных чисел. 
            // Записать все эти числа в файл. 
            // Вывести на экран все положительные компоненты файла.
            try
            {
                if (File.Exists(path))
                {
                    File.Delete(path);
                }

                // запись последовательности
                using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
                {
                    Console.Write("\nПоследовательность:");
                    for (int i = 1; i <= n; i++)
                    {
                        int a = rnd.Next(-9, 10);
                        double item = Convert.ToDouble(a) + Math.Round(rnd.NextDouble(), 3);
                        Console.Write(" {0}", item);
                        writer.Write(item);
                    }
                    Console.WriteLine("");
                }
                // чтение последовательности
                using (BinaryReader reader = new BinaryReader(File.Open(path, FileMode.Open)))
                {
                    Console.Write("Содержимое файла:");
                    // пока не достигнут конец файла, считываем каждое значение
                    while (reader.PeekChar() > -1)
                    {
                        double item = reader.ReadDouble();
                        if (item > 0.0)
                        {
                            Console.Write(" {0}", item);
                        }
                    }
                    Console.WriteLine("");
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        static void BinFileTask()
        {
            Console.WriteLine("\nРабота с двоичными файлами.");

            Console.Write("\nВведите n (размер последовательности): ");

            int n = Convert.ToInt32(Console.ReadLine());
            //int n = 10;

            ReverseNaturalSeq(n, @"D:\HW\projects\1365013 C# labs\lab10\file1.bin");
            IntSeq(n, @"D:\HW\projects\1365013 C# labs\lab10\file2.bin");
            RealSeq(n, @"D:\HW\projects\1365013 C# labs\lab10\file3.bin");

        }

        static void NetworkTask()
        {
            Console.WriteLine("\nРабота с сетью.");
            try
            {
                // Создайте приложение читающее содержимое веб-страницы по введенному адресу 
                // и размещающее ее в текстовый файл.
                
                //Console.Write("Введите адрес сайта: ");
                //string url = Console.ReadLine();
                //string url = "http://www.example.com";
                string url = "http://www.mail.ru";

                /*using (var client = new WebClient())
                {
                    string result = client.DownloadString(url);
                    Console.WriteLine(result);
                    Console.WriteLine("Headers: {0}", client.Headers.ToString());
                    
                }*/
                var req = WebRequest.Create(url);
                req.BeginGetResponse(r =>
                {
                    var response = req.EndGetResponse(r);
                    var stream = response.GetResponseStream();
                    var reader = new StreamReader(stream, true);
                    var str = reader.ReadToEnd();
                    //Console.WriteLine(str);
                    using (StreamWriter sw = new StreamWriter(@"D:\HW\projects\1365013 C# labs\lab10\web.txt", false, System.Text.Encoding.Default))
                    {
                        sw.WriteLine(str);
                    }
                    // Поместите в текстовый файл информацию о хосте и всех адресах mail.ru
                    using (StreamWriter sw = new StreamWriter(@"D:\HW\projects\1365013 C# labs\lab10\headers.txt", false, System.Text.Encoding.Default))
                    {
                        sw.WriteLine(response.Headers.ToString());
                    }
                }, null);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        static void TextFileTask()
        {
            Console.WriteLine("\nРабота с текстовым (символьным) файлом.");

            string path = @"D:\HW\projects\1365013 C# labs\lab10\input.txt";
            //string path = "input.txt"

            try
            {
                // Дан текстовый файл. Найти самую длинную строку и ее длину.
                using (StreamReader sr = new StreamReader(path))
                {
                    string sMax = "";
                    while (sr.Peek() >= 0)
                    {
                        string s = sr.ReadLine();
                        if (s.Length > sMax.Length)
                        {
                            sMax = s;
                        }
                    }
                    Console.WriteLine("\nСамая длинная строка в файле: \"{0}\" ({1} символов)", sMax, sMax.Length);
                }

                // Дан текстовый файл. Найти номер самой короткой строки.
                using (StreamReader sr = new StreamReader(path))
                {
                    bool first = true;
                    int n = 0;
                    int nMin = 0;
                    string sMin = "";
                    while (sr.Peek() >= 0)
                    {
                        n++;
                        string s = sr.ReadLine();
                        if (first || s.Length < sMin.Length)
                        {
                            first = false;
                            sMin = s;
                            nMin = n;
                        }
                    }
                    Console.WriteLine("\nСамая короткая строка в файле: \"{0}\" (находится на {1}й строке)", sMin, nMin);
                }
                // Дан текстовый файл. Напечатать все строки, длина которых равна данному числу.
                int len = 5;
                Console.WriteLine("\nСтроки длины {0}:", len);
                using (StreamReader sr = new StreamReader(path))
                {
                    while (sr.Peek() >= 0)
                    {
                        string s = sr.ReadLine();
                        if (s.Length == len)
                        {
                            Console.WriteLine(s);
                        }
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        static void Main(string[] args)
        {
            Console.WriteLine("Лабораторная работа №10");
            BinFileTask();
            TextFileTask();
            NetworkTask();

            Console.Write("\nНажмите любую клавишу...");
            Console.ReadKey();
        }
    }
}
