using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace OS_lab3
{
    public partial class Plots: UserControl
    {
        private Background controllerBackground;
        private Foreground controllerForeground;

        public Plots()
        {
            controllerBackground = new Background();
            controllerForeground = new Foreground();
            InitializeComponent();
            controllerBackground.Tick = 500;
            controllerForeground.Tick = 500;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            double averageWait;
            double procent;
            List<Task> listOfTasks = new List<Task>();
            List<Task> listOfTasks2 = new List<Task>();
            List<Task> listOfTasks3 = new List<Task>();
            List<Task> listOfTasks4 = new List<Task>();
            var chartFirst = chart1.ChartAreas[0];
            var chartSecond = chart2.ChartAreas[0];
            var chartThird = chart3.ChartAreas[0];

            chartFirst.AxisX.IntervalType = DateTimeIntervalType.Number;
            chartSecond.AxisX.IntervalType = DateTimeIntervalType.Number;
            chartThird.AxisX.IntervalType = DateTimeIntervalType.Number;

            chartFirst.AxisX.LabelStyle.Format = "";
            chartFirst.AxisY.LabelStyle.Format = "";
            chartFirst.AxisX.Interval = 0.00007;
            chartFirst.AxisY.Interval = 0.5;
            chart1.Series.Add("залежність середнього часу очікування від інтенсивності вхідного потоку заявок");
            chart1.Series["залежність середнього часу очікування від інтенсивності вхідного потоку заявок"].ChartType = SeriesChartType.Spline;
            chart1.Series["залежність середнього часу очікування від інтенсивності вхідного потоку заявок"].Color = Color.Red;
            chartSecond.AxisX.LabelStyle.Format = "";
            chartSecond.AxisY.LabelStyle.Format = "";
            chartSecond.AxisX.Interval = 0.0007;
            chartSecond.AxisY.Interval = 0.05;
            chart2.Series.Add("залежність проценту простою ресурсу від  інтенсивності вхідного потоку заявок");
            chart2.Series["залежність проценту простою ресурсу від  інтенсивності вхідного потоку заявок"].ChartType = SeriesChartType.Spline;
            chart2.Series["залежність проценту простою ресурсу від  інтенсивності вхідного потоку заявок"].Color = Color.Blue;
            chartThird.AxisX.LabelStyle.Format = "";
            chartThird.AxisY.LabelStyle.Format = "";
            chartThird.AxisX.Interval = 0.01;
            chartThird.AxisY.Interval = 0.1;
            chart3.Series.Add("залежність кількості заявок від часу очікування при фіксованій  інтенсивності вхідного потоку заявок");
            chart3.Series["залежність кількості заявок від часу очікування при фіксованій  інтенсивності вхідного потоку заявок"].ChartType = SeriesChartType.Spline;
            chart3.Series["залежність кількості заявок від часу очікування при фіксованій  інтенсивності вхідного потоку заявок"].Color = Color.Green;

            for (int i = 1; i<=32; i++ )
            {
                controllerBackground.FreqFrom = i;
                controllerBackground.FreqTo = i;
                controllerForeground.FreqFrom = i;
                controllerForeground.FreqTo = i;
                controllerBackground.MakeOneTick();
                controllerForeground.MakeOneTick();
                listOfTasks = controllerBackground.ListOfTasks;
                listOfTasks2 = listOfTasks.Concat(controllerForeground.ListOfTasks).ToList();
                averageWait = listOfTasks2.Average(task => task.Wait);
                procent = ((double)listOfTasks2.FindAll(task => task.State == "виконано").Count / 500) * 100 ;
                chart1.Series["залежність середнього часу очікування від інтенсивності вхідного потоку заявок"].Points.AddXY(500 / -i, averageWait);
                chart2.Series["залежність проценту простою ресурсу від  інтенсивності вхідного потоку заявок"].Points.AddXY(500 / i, procent);
            }
            double averageWait2;

            for (int i = 0; i < 32; i++)
            {
                controllerBackground.FreqFrom = 10;
                controllerBackground.FreqTo = 10;
                controllerForeground.FreqFrom = 10;
                controllerForeground.FreqTo = 10;
                controllerBackground.MakeOneTick();
                controllerForeground.MakeOneTick();
                listOfTasks3 = controllerBackground.ListOfTasks;
                listOfTasks4 = listOfTasks3.Concat(controllerForeground.ListOfTasks).ToList();
                averageWait2 = listOfTasks4.Average(task => task.Wait);
                chart3.Series["залежність кількості заявок від часу очікування при фіксованій  інтенсивності вхідного потоку заявок"].Points.AddXY(averageWait2, listOfTasks4.FindAll(task => task.State == "виконано").Count);
            }
            
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }
}
