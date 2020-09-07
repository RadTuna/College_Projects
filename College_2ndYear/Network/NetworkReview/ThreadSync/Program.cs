using System;
using System.Threading;

class Program
{
    private int number = 0;

    static void Main(string[] args)
    {
        Program pg = new Program();

        Thread t1 = new Thread(threadFunc);
        Thread t2 = new Thread(threadFunc);

        t1.Start(pg);
        t2.Start(pg);

        t1.Join();
        t2.Join();

        Console.WriteLine(pg.number);
    }

    static void threadFunc(object inst)
    {
        Program pg = inst as Program;
        if (pg != null)
        {
            for (int i = 0; i < 10000; ++i)
            {
                Interlocked.Increment(ref pg.number);
            }
        }
    }
}
