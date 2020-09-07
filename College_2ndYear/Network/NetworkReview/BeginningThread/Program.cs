using System;
using System.Threading;
using Microsoft.Win32.SafeHandles;

class ThreadSample
{

    private readonly Object counter1Lock = new Object();
    private readonly Object counter2Lock = new Object();

    public static void Main()
    {
        ThreadSample sample = new ThreadSample();
    }

    public ThreadSample()
    {
        Thread newCounter = new Thread(new ThreadStart(Counter));
        Thread newCounter2 = new Thread(new ThreadStart(Counter2));

        newCounter.Start();
        newCounter2.Start();

        for (int i = 0; i < 10; ++i)
        {
            Console.WriteLine("Main : {0}", i);
            Thread.Sleep(1000);

            lock (counter1Lock)
            {
                if (i % 2 == 0)
                {
                    Monitor.Pulse(counter1Lock);
                }
            }

            lock (counter2Lock)
            {
                if (i % 3 == 0)
                {
                    Monitor.Pulse(counter2Lock);
                }
            }
        }

        newCounter.Interrupt(); 
        newCounter2.Interrupt();
    }

    void Counter()
    {
        for (int i = 0; i < 10; ++i)
        {
            lock (counter1Lock)
            {
                try
                {
                    Monitor.Wait(counter1Lock);
                }
                catch (ThreadInterruptedException e)
                {
                    Console.WriteLine("Counter thread interrupted.");
                    return;
                }

                Console.WriteLine(" Thread : {0}", i);
            }
        }
    }

    void Counter2()
    {
        for (int i = 0; i < 10; ++i)
        {
            lock (counter2Lock)
            {
                try
                {
                    Monitor.Wait(counter2Lock);
                }
                catch (ThreadInterruptedException e)
                {
                    Console.WriteLine("Counter1 thread interrupted.");
                    return;
                }

                Console.WriteLine(" Thread2 : {0}", i);
            }
        }
    }
}
