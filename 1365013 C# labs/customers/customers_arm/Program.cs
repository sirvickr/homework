﻿using System;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace customers_arm
{
    class Program
    {
        // адрес и порт сервера, к которому будем подключаться
        static int port = 8887; // порт сервера
        static string address = "127.0.0.1"; // адрес сервера

        static void Main(string[] args)
        {
            try
            {
                IPEndPoint ipPoint = new IPEndPoint(IPAddress.Parse(address), port);

                Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                // подключаемся к удаленному хосту
                socket.Connect(ipPoint);

                byte[] data = null;
                Console.Write("Введите имя покупателя: ");
                string request = Console.ReadLine();
                data = Encoding.Unicode.GetBytes(request);
                socket.Send(data);

                // получаем ответ
                data = new byte[256]; // буфер для ответа
                StringBuilder builder = new StringBuilder();
                int bytes = 0; // количество полученных байт
                do
                {
                    bytes = socket.Receive(data, data.Length, 0);
                    builder.Append(Encoding.Unicode.GetString(data, 0, bytes));
                }
                while (socket.Available > 0);
                Console.WriteLine("id покупателя: " + builder.ToString());

                // закрываем сокет
                socket.Shutdown(SocketShutdown.Both);
                socket.Close();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            Console.Write("Нажмите любую клавишу...");
            Console.ReadKey();
        }
    }
}
