using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace IOCPChatClient
{
    public class StateObject
    {
        public const int BUFFER_SIZE = 1024;
        public byte[] buffer = new byte[BUFFER_SIZE];
        public StringBuilder stringBuilder = new StringBuilder();
        public Socket serverSocket = null;
    }

    class Program
    {
        private static readonly int PORT = 9000;
        private static readonly int BUFFER_SIZE = 1024;
        private static byte[] mSendBuffer = new byte[BUFFER_SIZE];

        static void Main(string[] args)
        {
            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Loopback, PORT);
            Socket serverSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            try
            {
                serverSocket.Connect(serverEndPoint);
            }
            catch (SocketException e)
            {
                DisconnectSocket(serverSocket, serverEndPoint);
                return;
            }

            Console.WriteLine("Connected to server. IP: {0}, PORT: {1}", serverEndPoint.Address.ToString(), serverEndPoint.Port);
            
            StateObject stateObject = new StateObject();
            stateObject.serverSocket = serverSocket;

            serverSocket.BeginReceive(
                stateObject.buffer, 
                0, 
                StateObject.BUFFER_SIZE, 
                0, 
                ProcessRead, 
                stateObject);

            while (true)
            {
                string input = Console.ReadLine();
                if (input.Length == 0)
                {
                    Console.WriteLine("Disconnected...");
                    serverSocket.Shutdown(SocketShutdown.Both);
                    serverSocket.Close();
                    return;
                }

                
                mSendBuffer = Encoding.UTF8.GetBytes(input + "<<EOF>>");
                try
                {
                    serverSocket.Send(mSendBuffer);
                }
                catch (SocketException e)
                {
                    DisconnectSocket(serverSocket, serverEndPoint);
                }
            }
        }

        public static void ProcessRead(IAsyncResult asyncResult)
        {
            StateObject stateObject = asyncResult.AsyncState as StateObject;
            Debug.Assert(stateObject != null);

            Socket socket = stateObject.serverSocket;
            IPEndPoint endPoint = socket.RemoteEndPoint as IPEndPoint;
            Debug.Assert(endPoint != null);

            int receivedByte = 0;
            try
            {
                receivedByte = socket.EndReceive(asyncResult);
            }
            catch (SocketException e)
            {
                DisconnectSocket(socket, endPoint);
                return;
            }

            if (receivedByte > 0)
            {
                stateObject.stringBuilder.Append(
                    Encoding.UTF8.GetString(stateObject.buffer, 0, receivedByte));

                string content = stateObject.stringBuilder.ToString();
                int eofIndex = content.IndexOf("<<EOF>>", StringComparison.Ordinal);
                if (eofIndex > -1)
                {
                    content = content.Remove(eofIndex);
                    Console.WriteLine("[Other] {0}", content);
                }
                else
                {
                    socket.BeginReceive(
                        stateObject.buffer,
                        0,
                        StateObject.BUFFER_SIZE,
                        0,
                        ProcessRead,
                        stateObject);
                }
            }

            stateObject.stringBuilder.Clear();
            socket.BeginReceive(
                stateObject.buffer,
                0,
                StateObject.BUFFER_SIZE,
                0,
                ProcessRead,
                stateObject);
        }

        private static void DisconnectSocket(Socket socket, IPEndPoint endPoint)
        {
            Console.WriteLine("Server disconnected. IP:{0}, PORT:{1}", endPoint.Address.ToString(), endPoint.Port);
            socket.Close();
        }
    }
}
