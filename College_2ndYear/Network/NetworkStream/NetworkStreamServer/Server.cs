using System;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Reflection;

namespace NetworkStreamServer
{
    class Server
    {
        static void Main(string[] args)
        {
            Debug.Assert(args.Length == 1);

            string filePath = args[0];

            if (!File.Exists(filePath))
            {
                Console.WriteLine("Do not exists file.");
                return;
            }

            FileStream fileStream = File.OpenRead(filePath);

            IPEndPoint ipEndPoint = new IPEndPoint(IPAddress.Any, 9050);
            Socket serverSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            serverSocket.Bind(ipEndPoint);
            serverSocket.Listen(10);
            Console.WriteLine("Waiting for a client...");

            Socket clientSocket = serverSocket.Accept();
            IPEndPoint clientEndPoint = (IPEndPoint) clientSocket.RemoteEndPoint;
            Console.WriteLine("Connected with {0} at port {1}", clientEndPoint.Address, clientEndPoint.Port);

            NetworkStream networkStream = new NetworkStream(clientSocket);
            StreamReader streamReader = new StreamReader(fileStream);
            StreamWriter streamWriter = new StreamWriter(networkStream);

            string fileData = streamReader.ReadLine();
            while (fileData != null)
            { 
                streamWriter.WriteLine(fileData);
                fileData = streamReader.ReadLine();
            }

            Console.WriteLine("File transfer complete.");

            streamWriter.Close();
            streamReader.Close();
            networkStream.Close();
            fileStream.Close();
        }
    }
}
