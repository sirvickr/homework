using System;
using System.Net;
using System.Net.Sockets;

namespace tcp_server
{
    class Program
    {
        static int port = 8889; // порт для приема входящих соединений
        static void Main(string[] args)
        {
            // получаем локальный адрес для запуска сокета
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
                    Console.WriteLine(DateTime.Now.ToShortTimeString() + ": соединение принято");
                    // отправляем код подтвержения
                    byte[] ack = new byte[] { 0x01, 0x02, 0x03, 0x04 };
                    handler.Send(ack);
                    // закрываем сокет
                    handler.Shutdown(SocketShutdown.Both);
                    handler.Close();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
    }
}
