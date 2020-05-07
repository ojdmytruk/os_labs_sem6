using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OS_lab3
{
    public abstract class QueueController
    {

        public int SystemWaitsGenTime;
        protected int CurrentTime;

        private int _nextAppears;

        
        public int CurrentTick = -1;

        public int Tick;
        public int WeightFrom;
        public int WeightTo;
        public int FreqFrom;
        public int FreqTo;

        private readonly string _kind;

        public List<Task> ListOfTasks = new List<Task>();
        public List<string> ListOfSequence = new List<string>();
        protected readonly Random Rnd = new Random();

        protected QueueController(string kind)
        {
            _nextAppears = Rnd.Next(FreqFrom, FreqTo);
            _kind = kind;
        }

        protected void CreateTask()
        {
            while (true)
            {
                if (_nextAppears > CurrentTime + Tick || Tick < 1)
                    return;
                int nextWeight = Rnd.Next(WeightFrom, WeightTo + 1);
                ListOfTasks.Add(new Task()
                {
                    Appear = _nextAppears,
                    Start = -1,
                    Finish = -1,
                    Kind = _kind,
                    State = "очікування",
                });
                _nextAppears += Rnd.Next(FreqFrom, FreqTo + 1);
            }
        }

        protected abstract void ExecuteTask();

        protected abstract void UpdateWaitOption(int waitTime, int except, int timeAppear);

        public void MakeOneTick()
        {
            ListOfSequence.Clear();
            ListOfSequence.Add(_kind);
            CurrentTick++;
            CurrentTime = CurrentTick * Tick;

            UpdateWaitOption(Convert.ToInt16(Tick * (_kind == "foregrground" ? .8 : .2)), -1, CurrentTime);

            CreateTask();
            ExecuteTask();
        }
    }
}
