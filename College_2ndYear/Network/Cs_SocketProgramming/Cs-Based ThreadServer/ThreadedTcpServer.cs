using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace Cs_Based_ThreadServer
{
    class ThreadedTcpServer
    {
        private TcpListener client;

        public ThreadedTcpServer()
        {
            client = new TcpListener(IPAddress.Any, 9000);
            client.Start();

            Console.WriteLine("Waiting for clients...");

            while (true)
            {
                while (!client.Pending())
                {
                    Thread.Sleep(1000);
                }

                ConnectionThread newConnection = new ConnectionThread();
                newConnection.threadListener = client;

                Thread newThread = new Thread(new ThreadStart(newConnection.HandleConnection));
                newThread.Start();
            }
        }
    }
}
