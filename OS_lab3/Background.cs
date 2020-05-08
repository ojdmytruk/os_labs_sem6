using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OS_lab3
{
    public class Background : QueueController
    {
        private const double ExecutePart = 0.2;

        public Background() : base("background")
        {

        }

        protected override void ExecuteTask()
        {
            CurrentTime += Convert.ToInt16(Tick * (1 - ExecutePart));
            var tempTime = CurrentTime;

            while (tempTime < CurrentTime + Convert.ToInt16(Tick * ExecutePart))
            {
                var task = ListOfTasks.Find(t => t.State != "виконано" && t.Appear < tempTime);

                // якщо в черзі немає заявок, що очікують виконання
                if (null == task)
                {
                    SystemWaitsGenTime++;
                    tempTime++;
                    continue;
                }

                ListOfSequence.Add(task.Index.ToString());

                // якщо заявка знаходиться в стані очікування
                if (task.State == "очікування")
                {
                    task.Start = tempTime; //почати обробку
                    task.State = "обробка";
                }

                // якщо залишилось менше часу, ніж потрібно для виконання заявки
                if (task.LeftToProcess > CurrentTime + Convert.ToInt16(Tick * ExecutePart) - tempTime)
                {
                    task.LeftToProcess -= CurrentTime + Convert.ToInt16(Tick * ExecutePart) - tempTime;
                    return;
                }

                tempTime += task.LeftToProcess;
                task.LeftToProcess = 0;
                task.Finish = tempTime;
                task.State = "виконано";
            }
        }

        protected override void UpdateWaitOption(int waitTime, int exceptIdx, int timeAppear)
        {
            foreach (var task in ListOfTasks.Where(t => t.State != "виконано" && t.Index != exceptIdx))
                task.Wait += waitTime;
        }
    }
}
