using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace Cs_Based_UDPServerAndClient
{
    class Program
    {
        static void Main(string[] args)
        {
            Thread serverThread = new Thread(serverFunc);
            serverThread.IsBackground = true;
            serverThread.Start();
            Thread.Sleep(500);

            Thread clientThread = new Thread(clientFunc);
            clientThread.IsBackground = true;
            clientThread.Start();
            Thread.Sleep(500);

            Console.WriteLine("Press any key for exit...");
            Console.ReadLine();
        }

        private static void serverFunc(object obj)
        {
            Socket serverSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            IPEndPoint endPoint = new IPEndPoint(IPAddress.Any, 10200);

            serverSocket.Bind(endPoint);

            byte[] recvBytes = new byte[1024];

            EndPoint clientEP = new IPEndPoint(IPAddress.None, 0);

            while (true)
            {
                int nRecv = serverSocket.ReceiveFrom(recvBytes, ref clientEP);
                string text = Encoding.UTF8.GetString(recvBytes);

                byte[] sendBytes = Encoding.UTF8.GetBytes(text);
                serverSocket.SendTo(sendBytes, clientEP);
            }
        }

        private static void clientFunc(object obj)
        {
            Socket clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            EndPoint serverEP = new IPEndPoint(IPAddress.Loopback, 10200);
            EndPoint senderEP = new IPEndPoint(IPAddress.None, 0);

            int nTimes = 5;

            while (nTimes-- > 0)
            {
                byte[] buffer = Encoding.UTF8.GetBytes(DateTime.Now.ToString());
                clientSocket.SendTo(buffer, serverEP);

                byte[] recvBytes = new byte[1024];
                int nRecv = clientSocket.ReceiveFrom(recvBytes, ref senderEP);

                string text = Encoding.UTF8.GetString(recvBytes, 0, nRecv);
                Console.WriteLine(text);
                Thread.Sleep(1000);
            }

            clientSocket.Close();
            Console.WriteLine("UDP Client socket : Closed.");
        }
    }
}
