using System;
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
                byte[] data = null;
                int bytes = 0; // количество полученных байт
                StringBuilder builder = new StringBuilder();

                IPEndPoint ipPoint = new IPEndPoint(IPAddress.Parse(address), port);

                Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                // подключаемся к удаленному хосту
                socket.Connect(ipPoint);

                // ждём подтверждения
                data = new byte[256]; // буфер для ответа
                do
                {
                    bytes = socket.Receive(data, data.Length, 0);
                }
                while (socket.Available > 0);
                // проверяем ответ сервера
                Console.WriteLine("ответ сервера: " + bytes + " байт: " + data[0] + " " + data[1] + " " + data[2] + " " + data[3]);
                if (bytes == 4 && data[0] == 0x01 && data[1] == 0x02 && data[2] == 0x03 && data[3] == 0x04)
                {
                    Console.WriteLine("соединение подтверждено");

                    Console.Write("Введите имя покупателя: ");
                    string request = Console.ReadLine();
                    data = Encoding.Unicode.GetBytes(request);
                    socket.Send(data);

                    // получаем ответ
                    data = new byte[256]; // буфер для ответа
                    bytes = 0; // количество полученных байт
                    do
                    {
                        bytes = socket.Receive(data, data.Length, 0);
                        builder.Append(Encoding.Unicode.GetString(data, 0, bytes));
                    }
                    while (socket.Available > 0);
                    Console.WriteLine("id покупателя: " + builder.ToString());
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
            Console.Write("Нажмите любую клавишу...");
            Console.ReadKey();
        }
    }
}
