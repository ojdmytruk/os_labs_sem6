using System;
using System.Diagnostics;
using System.Threading;

namespace OS_Lab5
{
    class Program
    {
        static void Main(string[] args)
        {
            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();

            int[,,] b = new int[100, 100, 100];

            for (int j = 0; j < 100; j++)
            {
                for (int i = 0; i < 100; i++)
                {
                    for (int k = 0; k < 100; k++)
                    {
                        b[j, i, k]++;
                    }
                }
            }
            stopwatch.Stop();
            TimeSpan interval = TimeSpan.FromTicks(stopwatch.ElapsedTicks);
            string timeInterval = interval.ToString();
            Console.WriteLine("Час виконання" + timeInterval);
        }
    }
}
