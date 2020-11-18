using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace IOCPChatServer
{
    public class StateObject
    {
        public const int BUFFER_SIZE = 1024;
        public byte[] buffer = new byte[BUFFER_SIZE];
        public StringBuilder stringBuilder = new StringBuilder();
        public Socket clientSocket = null;
    }

    class Program
    {
        private static readonly int PORT = 9000;
        private static List<Socket> mConnectedClients = new List<Socket>();
        private static Object mLockObject = new Object();

        static void Main(string[] args)
        {
            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Any, PORT);
            Socket serverSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            serverSocket.Bind(serverEndPoint);
            serverSocket.Listen(100);

            Console.WriteLine("Waiting for client connection...");
            serverSocket.BeginAccept(ProcessAccept, serverSocket);

            while (true)
            {
                Console.WriteLine("If you need server disconnection. Type \'Exit\'.");
                string input = Console.ReadLine();
                if (input == "Exit")
                {
                    break;
                }
            }

            if (serverSocket.Connected)
            {
                serverSocket.Shutdown(SocketShutdown.Both);
            }

            serverSocket.Close();
        }

        public static void ProcessAccept(IAsyncResult asyncResult)
        {
            Socket serverSocket = asyncResult.AsyncState as Socket;
            Debug.Assert(serverSocket != null);

            Socket clientSocket = serverSocket.EndAccept(asyncResult);

            IPEndPoint clientEndPoint = clientSocket.RemoteEndPoint as IPEndPoint;
            Debug.Assert(clientEndPoint != null);

            StateObject clientStateObject = new StateObject();
            clientStateObject.clientSocket = clientSocket;

            lock (mLockObject)
            {
                mConnectedClients.Add(clientSocket);
            }

            Console.WriteLine("Client connected. IP:{0}, PORT:{1}", clientEndPoint.Address.ToString(),
                clientEndPoint.Port);

            clientSocket.BeginReceive(
                clientStateObject.buffer,
                0,
                StateObject.BUFFER_SIZE,
                0,
                ProcessRead,
                clientStateObject);

            Console.WriteLine("Waiting for client connection...");
            serverSocket.BeginAccept(ProcessAccept, serverSocket);
        }

        public static void ProcessRead(IAsyncResult asyncResult)
        {
            StateObject stateObject = asyncResult.AsyncState as StateObject;
            Debug.Assert(stateObject != null);

            Socket socket = stateObject.clientSocket;
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
                    string convertedString = content.Remove(eofIndex);
                    Console.WriteLine("[{0} : {1}] {2}", endPoint.Address.ToString(), endPoint.Port, convertedString);

                    SendAll(content, stateObject);
                }
                else
                {
                    socket.BeginReceive(stateObject.buffer,
                        0,
                        StateObject.BUFFER_SIZE,
                        0,
                        ProcessRead,
                        stateObject);
                }
            }
            else
            {
                DisconnectSocket(socket, endPoint);
            }
        }

        public static void SendAll(String content, StateObject stateObject)
        {
            byte[] byteData = Encoding.UTF8.GetBytes(content);
            lock (mLockObject)
            {
                foreach (Socket socket in mConnectedClients)
                {
                    if (socket == stateObject.clientSocket)
                    {
                        continue;
                    }

                    socket.BeginSend(
                            byteData,
                            0,
                            byteData.Length,
                            0,
                            ProcessSend,
                            socket);
                }
            }

            stateObject.stringBuilder.Clear();
            stateObject.clientSocket.BeginReceive(stateObject.buffer,
                0,
                StateObject.BUFFER_SIZE,
                0,
                ProcessRead,
                stateObject);
        }

        public static void ProcessSend(IAsyncResult asyncResult)
        {
            Socket socket = asyncResult.AsyncState as Socket;
            Debug.Assert(socket != null);

            IPEndPoint endPoint = socket.RemoteEndPoint as IPEndPoint;
            Debug.Assert(endPoint != null);

            int sentByte = 0;
            try
            {
                sentByte = socket.EndSend(asyncResult);
            }
            catch (SocketException e)
            {
                DisconnectSocket(socket, endPoint);
                return;
            }

            Console.WriteLine("[{0} : {1}] Sent byte - {2}byte", endPoint.Address.ToString(), endPoint.Port, sentByte);
        }

        private static void DisconnectSocket(Socket socket, IPEndPoint endPoint)
        {
            Console.WriteLine("Client disconnected. IP:{0}, PORT:{1}", endPoint.Address.ToString(), endPoint.Port);
            lock (mLockObject)
            {
                mConnectedClients.Remove(socket);
            }

            if (socket.Connected)
            {
                socket.Shutdown(SocketShutdown.Both);
            }

            socket.Close();
        }
    }
}
