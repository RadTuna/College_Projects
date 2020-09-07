using System;
using System.Runtime.InteropServices.ComTypes;
using System.Threading;

class Program
{
    static void Main(string[] args)
    {
        Thread t = new Thread(threadFunc);
        t.IsBackground = true;

        t.Start();
        Thread.Sleep(1000);

        Console.WriteLine("메인 스레드 종료!");
    }

    static void threadFunc()
    {
        Console.WriteLine("10초 후에 프로그램 종료!");
        for (int i = 10; i > 0; --i)
        {
            Console.WriteLine("남은시간 : {0}초", i);
            Thread.Sleep(1000);
        }
        Console.WriteLine("스레드 종료!");
    }
}