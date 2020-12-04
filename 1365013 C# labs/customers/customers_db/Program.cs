using System;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Collections.Generic;
using System.IO;

namespace customers_db
{
    class Program
    {
        static int port = 8887; // порт для приема входящих запросов

        static void Main(string[] args)
        {
            // Загружаем базу данных покупателей
            string path = @"..\..\db.txt";
            //List<Customer> customers = new List<Customer>();
            Dictionary<string, int> customers = new Dictionary<string, int>();
            using (StreamReader sr = new StreamReader(path))
            {
                Console.WriteLine("customers:");
                int n = 0;
                while (sr.Peek() >= 0)
                {
                    n++;
                    string line = sr.ReadLine();
                    string[] fields = line.Split(new char[2] { ' ', '\t' });
                    if(fields.Length != 2)
                    {
                        Console.WriteLine("Неизвестный формат в строке {0}", n);
                        continue;
                    }
                    customers.Add(fields[1], Convert.ToInt32(fields[0]));
                }
                foreach(KeyValuePair<string, int> customer in customers)
                {
                    Console.WriteLine("{0}\t{1}", customer.Value, customer.Key);
                }
            }

            // получаем адреса для запуска сокета
            IPEndPoint ipPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), port);
            // создаем сокет
            Socket listenSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                // связываем сокет с локальной точкой, по которой будем принимать данные
                listenSocket.Bind(ipPoint);
                // начинаем прослушивание
                listenSocket.Listen(10);
                Console.WriteLine("Сервер запущен. Ожидание подключений...");
                // цикл приёма соединений
                while (true)
                {
                    Socket handler = listenSocket.Accept();

                    // отправляем код подтвержения
                    byte[] ack = new byte[] { 1, 2, 3, 4 };
                    handler.Send(ack);

                    // получаем запрос
                    StringBuilder builder = new StringBuilder();
                    int bytes = 0; // количество полученных байтов
                    byte[] data = new byte[256]; // буфер для получаемых данных
                    do
                    {
                        bytes = handler.Receive(data);
                        builder.Append(Encoding.Unicode.GetString(data, 0, bytes));
                    }
                    while (handler.Available > 0);

                    string name = builder.ToString();
                    // пишем в журнал о принятом запросе
                    Console.WriteLine(DateTime.Now.ToShortTimeString() + "\tзапрос на покупателя: " + name);
                    
                    // отправляем ответ
                    string message = null;
                    if (customers.ContainsKey(name))
                        message = customers[name].ToString();
                    else
                        message = "покупатель не найден в БД";
                    data = Encoding.Unicode.GetBytes(message);
                    handler.Send(data);
                    
                    // закрываем сокет
                    handler.Shutdown(SocketShutdown.Both);
                    handler.Close();
                } // цикл приёма соединений
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
    }
}
