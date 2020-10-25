using System;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace NetworkStreamClient
{
    class Client
    {
        static void Main(string[] args)
        {
            Debug.Assert(args.Length == 1);

            string newFilePath = args[0];
            FileStream fileStream = File.OpenWrite(newFilePath);

            IPEndPoint ipEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 9050);
            Socket server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            try
            {
                server.Connect(ipEndPoint);
            }
            catch (SocketException e)
            {
                Console.Error.WriteLine("Unable to connect to server");
                Console.Error.WriteLine(e.ToString());
                return;
            }

            NetworkStream networkStream = new NetworkStream(server);
            StreamReader streamReader = new StreamReader(networkStream);
            StreamWriter streamWriter = new StreamWriter(fileStream);

            string dataLine = null;
            try
            {
                dataLine = streamReader.ReadLine();
            }
            catch (IOException e)
            {
                Console.Error.WriteLine(e.ToString());
                goto EXIT_FUNC;
            }

            while (dataLine != null)
            {
                streamWriter.WriteLine(dataLine);

                try
                {
                    dataLine = streamReader.ReadLine();
                }
                catch (IOException e)
                {
                    break;
                }
            }

            streamWriter.Flush();
            Console.WriteLine("File receive complete.");

            EXIT_FUNC:
            streamWriter.Close();
            streamReader.Close();
            networkStream.Close();
            fileStream.Close();
        }
    }
}
