using System;
using System.Collections.Generic;
using System.IO;
using System.Net.Sockets;
using System.Text;

namespace Cs_Based_ThreadServer
{
    class ConnectionThread
    {
        public TcpListener threadListener;
        private static int connections = 0;

        public void HandleConnection()
        {
            int recv;
            byte[] data = new byte[1024];

            TcpClient client = threadListener.AcceptTcpClient();
            NetworkStream networkStream = client.GetStream();
            ++connections;

            Console.WriteLine("New client accepted : {0} active connections", connections);

            string welcomeMessage = "Welcome to my test server";
            data = Encoding.ASCII.GetBytes(welcomeMessage);

            //networkStream.Write(data, 0, data.Length);

            while (true)
            {
                Array.Clear(data, 0, data.Length);

                try
                {
                    recv = networkStream.Read(data, 0, data.Length);
                    networkStream.Write(data, 0, recv);
                }
                catch (IOException exception)
                {
                    break;
                }
            }

            networkStream.Close();
            client.Close();
            --connections;

            Console.WriteLine("Client disconnected : {0} active connections", connections);
        }
    }
}
