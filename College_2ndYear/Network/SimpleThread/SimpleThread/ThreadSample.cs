using System;
using System.Threading;

namespace SimpleThread
{
    class ThreadSample
    {
        public ThreadSample()
        {
            Thread newCounter0 = new Thread(new ThreadStart(Counter0));
            Thread newCounter1 = new Thread(new ThreadStart(Counter1));

            newCounter0.Start();
            newCounter1.Start();

            for (int i = 0; i < 10; ++i)
            {
                Console.WriteLine("main : {0}", i);
                Thread.Sleep(1000);
            }
        }

        private void Counter0()
        {
            for (int i = 0; i < 10; ++i)
            {
                Console.WriteLine("    Thread0 : {0}", i);
                Thread.Sleep(1000);
            }
        }

        private void Counter1()
        {
            for (int i = 0; i < 10; ++i)
            {
                Console.WriteLine("    Thread1 : {0}", i);
                Thread.Sleep(1000);
            }
        }

    }
}
