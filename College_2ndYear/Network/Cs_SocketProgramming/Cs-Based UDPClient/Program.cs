using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace Cs_Based_UDPClient
{
    class Program
    {
        static void Main(string[] args)
        {
            Socket clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            byte[] buffer = Encoding.UTF8.GetBytes(DateTime.Now.ToString());

            EndPoint serverEP = new IPEndPoint(IPAddress.Loopback, 10200);

            clientSocket.SendTo(buffer, serverEP);

            byte[] recvBytes = new byte[1024];
            int nRecv = clientSocket.ReceiveFrom(recvBytes, ref serverEP);
            string text = Encoding.UTF8.GetString(recvBytes, 0, nRecv);

            Console.WriteLine(text);
        }
    }
}
