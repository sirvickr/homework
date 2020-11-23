using System;
using System.Text;
using System.Net;
using System.Net.Sockets;

namespace tcp_client
{
    class Program
    {
        // адрес и порт сервера, к которому будем подключаться
        static int port = 8889; // порт сервера
        static string address = "127.0.0.1"; // адрес сервера (localhost)
        static void Main(string[] args)
        {
            try
            {
                IPEndPoint ipPoint = new IPEndPoint(IPAddress.Parse(address), port);
                // создаём экземпляр сокета
                Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                // подключаемся к удаленному хосту
                socket.Connect(ipPoint);
                byte[] data = null;
                // ждём подтверждения
                data = new byte[256]; // буфер для ответа
                int bytes = 0; // количество полученных байт
                do
                {
                    bytes = socket.Receive(data, data.Length, 0);
                }
                while (socket.Available > 0);
                // проверяем ответ сервера
                Console.WriteLine("ответ сервера: " + bytes + " байт: " + data[0] + " " + data[1] + " " + data[2] + " " + data[3]);
                if(bytes == 4 && data[0] == 0x01 && data[1] == 0x02 && data[2] == 0x03 && data[3] == 0x04)
                {
                    Console.WriteLine("соединение подтверждено");
                }
                else
                {
                    Console.WriteLine("неизвестный ответ сервера");
                }
                // закрываем сокет
                socket.Shutdown(SocketShutdown.Both);
                socket.Close();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            Console.Write("нажмите любую клавишу... ");
            Console.ReadKey();
        }
    }
}
