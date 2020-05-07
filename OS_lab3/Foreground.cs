using System;
using System.Linq;

namespace OS_lab3
{
    public class Foreground : QueueController
    {
        private const double ExecutePart = 0.8;
        private int _currentTask = -1;
        private int _leftOfQuantum;
        private int _quantum = 100;

        public Foreground() : base("foreground")
        {
            _leftOfQuantum = _quantum;
        }

        protected override void ExecuteTask()
        {
            var tempTime = CurrentTime;

            while (tempTime < CurrentTime + Convert.ToInt16(Tick * ExecutePart))
            {
                // якщо заявка не встигла завершитись за відведений квант часу
                Task task;
                if (_leftOfQuantum < _quantum && -1 != _currentTask)
                {
                    task = ListOfTasks.Find(t => t.Index == _currentTask);
                    ListOfSequence.Add(task.Index.ToString());

                    // якщо заявку можливо виконати за поточний квант часу
                    if (task.LeftToProcess < _leftOfQuantum)
                    {
                        tempTime += task.LeftToProcess;

                        UpdateWaitOption(task.LeftToProcess, task.Index, tempTime);
                        //виконати заявку
                        task.LeftToProcess = 0;
                        task.State = "виконано";
                        task.Finish = tempTime;
                    }

                    // У протилежному випадку, заявку не можна виконати за квант і вона додається в чергу очікування
                    else
                    {
                        tempTime += _leftOfQuantum;
                        task.LeftToProcess -= _leftOfQuantum;

                        UpdateWaitOption(_quantum, task.Index, tempTime);
                    }
                }
                _leftOfQuantum = _quantum;

                task = ListOfTasks.Find(t => t.State != "виконано" && t.Index > _currentTask && t.Appear < tempTime);

                // No task in a queue after index is found
                if (null == task)
                {
                    _currentTask = -1;
                    SystemWaitsGenTime++;
                    tempTime++;
                    continue;
                }

                ListOfSequence.Add(task.Index.ToString());
                _currentTask = task.Index;

                // якщо заявка стоїть в черзі на обробку
                if (task.State == "очікуваня")
                {
                    task.Start = tempTime;
                    //розпочати обробку
                    task.State = "обробка";
                    task.Wait = task.Start - task.Appear;
                }

                // якщо залишилось більше часу, ніж квант
                if (tempTime < CurrentTime + Tick * ExecutePart - _quantum)
                {
                    // якщо заявка потребує більше, ніж квант часу
                    if (task.LeftToProcess > _quantum)
                    {
                        task.LeftToProcess -= _quantum;
                        tempTime += _quantum;
                        
                        UpdateWaitOption(_quantum, task.Index, tempTime);
                    }

                    // Заявка не може бути виконана за квант часу
                    else
                    {
                        tempTime += task.LeftToProcess;

                        UpdateWaitOption(task.LeftToProcess, task.Index, tempTime);

                        task.LeftToProcess = 0;
                        task.State = "виконано";
                        task.Finish = tempTime;
                    }
                }

                // Якщо до завершення процессу залишилось менше , ніж квант часу
                else
                {

                    // Якщо заявці потрібно менше часу, ніж залишлось до завершення процесу - виконати
                    if (task.LeftToProcess < CurrentTime + Convert.ToInt16(Tick * ExecutePart) - tempTime)
                    {
                        tempTime += task.LeftToProcess;

                        UpdateWaitOption(task.LeftToProcess, task.Index, tempTime);

                        task.LeftToProcess = 0;
                        task.State = "виконано";
                        task.Finish = tempTime;
                    }

                    // У протилежному випадку заявку буде перервано
                    else
                    {
                        _leftOfQuantum = _quantum + tempTime - CurrentTime - Convert.ToInt16(Tick * ExecutePart);

                        task.LeftToProcess += _leftOfQuantum - _quantum;

                        UpdateWaitOption(_quantum - _leftOfQuantum,
                            task.Index, CurrentTime + Convert.ToInt16(Tick * ExecutePart));

                        _currentTask = task.Index;
                        return;
                    }
                }

            }
        }

        protected override void UpdateWaitOption(int waitTime, int exceptIdx, int timeAppear)
        {
            foreach (var task in ListOfTasks.Where(t => t.State != "виконано" && t.Index != exceptIdx && t.Appear < timeAppear))
                task.Wait += waitTime;
        }
    }
}
