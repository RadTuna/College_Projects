using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace Cs_Based_ThreadServer
{
    class Program
    {
        static void Main(string[] args)
        {
            ThreadedTcpServer server = new ThreadedTcpServer();
        }
    }
}
