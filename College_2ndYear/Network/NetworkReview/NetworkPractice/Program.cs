using System;
using System.Net;
using System.Net.Sockets;

class SockProp
{
    public static void Main(string[] args)
    {
        IPAddress ia = IPAddress.Parse("127.0.0.1");
        IPEndPoint ie = new IPEndPoint(ia, 8000);

        Socket test = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

        Console.WriteLine("AddressFamily : {0}", test.AddressFamily);
        Console.WriteLine("SocketType : {0}", test.SocketType);
        Console.WriteLine("ProtocolType : {0}", test.ProtocolType);

        Console.WriteLine("Blocking : {0}", test.Blocking);

        test.Blocking = false;
        Console.WriteLine("new Blocking : {0}", test.Blocking);
        Console.WriteLine("Connected : {0}", test.Connected);

        test.Bind(ie);
        IPEndPoint iep = test.LocalEndPoint as IPEndPoint;
        if (iep != null)
        {
            Console.WriteLine("Local EndPoint : {0}", iep.ToString());
        }

        test.Close();
    }
}
