using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace Cs_Based_UDPServer
{
    class Program
    {
        static void Main(string[] args)
        {
            Thread serverThread = new Thread(ServerFunc);
            serverThread.IsBackground = true;
            serverThread.Start();

            Thread.Sleep(500);

            Console.WriteLine("Press any key for exit...");
            Console.ReadLine();
        }

        private static void ServerFunc(object obj)
        {
            Socket serverSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            IPEndPoint endPoint = new IPEndPoint(IPAddress.Any, 10200);

            serverSocket.Bind(endPoint);

            byte[] recvBytes = new byte[1024];
            EndPoint clientEP = new IPEndPoint(IPAddress.None, 0);

            while (true)
            {
                int nRecv = serverSocket.ReceiveFrom(recvBytes, ref clientEP);
                string text = Encoding.UTF8.GetString(recvBytes, 0, nRecv);

                byte[] sendBytes = Encoding.UTF8.GetBytes("Hello: " + text);
                serverSocket.SendTo(sendBytes, clientEP);
            }
        }
    }
}
