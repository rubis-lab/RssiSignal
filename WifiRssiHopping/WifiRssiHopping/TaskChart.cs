using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using SpPerfChart;

namespace WifiRssiHopping
{
    public partial class TaskChart : PerfChart
    {        
        uint _maxValue = 0;

        DateTime _lastUpdate;

        public DateTime LastUpdate
        {
            get { return _lastUpdate; }
            set { _lastUpdate = value; }
        }


        bool _deadlineMiss = false;

        public bool DeadlineMiss
        {
            get { return _deadlineMiss; }
            set { _deadlineMiss = value; }
        }


        public TaskChart()
        {
            InitializeComponent();            
            Initialize();
        }
        
        public TaskChart(string taskName, uint id, uint deadline, uint maxY)
        {
            InitializeComponent();            
            Initialize();
        }

        int MAX = 150;

        public void Initialize()
        {
            ChartPen penHotPink = new ChartPen();
            penHotPink.Color = System.Drawing.Color.HotPink;
            penHotPink.DashStyle = System.Drawing.Drawing2D.DashStyle.Solid;
            penHotPink.Width = 1F;

            ChartPen penBlack = new ChartPen();
            penBlack.Color = System.Drawing.Color.Black;
            penBlack.DashStyle = System.Drawing.Drawing2D.DashStyle.Solid;
            penBlack.Width = 1F;

            ChartPen penSilverDot = new ChartPen();
            penSilverDot.Color = System.Drawing.Color.Silver;
            penSilverDot.DashStyle = System.Drawing.Drawing2D.DashStyle.Dot;
            penSilverDot.Width = 1F;
            
            BorderStyle = System.Windows.Forms.Border3DStyle.Flat;
            PerfChartStyle.AntiAliasing = true;
            PerfChartStyle.AvgLinePen = penHotPink;
            PerfChartStyle.BackgroundColorBottom = System.Drawing.Color.Pink;
            PerfChartStyle.BackgroundColorTop = System.Drawing.Color.LavenderBlush;
            PerfChartStyle.ChartLinePen = penBlack;
            PerfChartStyle.HorizontalGridPen = penSilverDot;
            PerfChartStyle.ShowAverageLine = false;
            PerfChartStyle.ShowCurrentMaxValue = false;
            PerfChartStyle.ShowHorizontalGridLines = true;
            PerfChartStyle.ShowVerticalGridLines = true;
            PerfChartStyle.VerticalGridPen = penSilverDot;
            ScaleMode = SpPerfChart.ScaleMode.Relative;
            TimerMode = SpPerfChart.TimerMode.Disabled;
        }

        public override void AddValue(decimal value)
        {
            base.AddValue(value);
            _maxValue = (uint)Math.Max(value, (decimal)_maxValue);
            _lastUpdate = DateTime.Now;
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            // Enable AntiAliasing, if needed
            if (PerfChartStyle.AntiAliasing)
                e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;

            // Draw backgound
            {
                if (false)
                {
                    PerfChartStyle.BackgroundColorTop = Color.LavenderBlush;
                    PerfChartStyle.BackgroundColorBottom = Color.Pink;
                }
                else
                {
                    PerfChartStyle.BackgroundColorTop = Color.Honeydew;
                    PerfChartStyle.BackgroundColorBottom = Color.PaleGreen;
                }

                DrawBackgroundAndGrid(e.Graphics);
            }

            // Draw current grid value string
            SolidBrush sb = new SolidBrush(perfChartStyle.ChartLinePen.Color);
            //e.Graphics.DrawString(string.Format("{0:N0}us", _task.MaxY), this.Font, sb, 1.0f, 0.0f);
            e.Graphics.DrawString("0us", this.Font, sb, 1.0f, this.Height - this.Font.Height);


            // Draw title
            Font fontBold = new Font(this.Font, FontStyle.Bold);
            SizeF sizeName = e.Graphics.MeasureString("TODO", fontBold);
            e.Graphics.DrawString("TODO", fontBold, new SolidBrush(PerfChartStyle.ChartLinePen.Color), (this.Width - sizeName.Width), 2.0f);


            // Draw deadline
            {
                int verticalPosition = GetVerticalPosition(MAX)- 2;
                e.Graphics.DrawLine(PerfChartStyle.AvgLinePen.Pen, 0, verticalPosition, Width, verticalPosition);

                string sDeadline = string.Format("{0:N0}us", 1);
                float horizontalPosition = 1.0f;
                if (verticalPosition + (this.Font.Height*2) > this.Height)
                {
                    verticalPosition -= this.Font.Height;
                    horizontalPosition = 2 + e.Graphics.MeasureString(sDeadline, this.Font).Width;
                }

                if (verticalPosition < this.Font.Height)
                    horizontalPosition = 2 + e.Graphics.MeasureString(sDeadline, this.Font).Width;

                e.Graphics.DrawString(sDeadline, this.Font, new SolidBrush(PerfChartStyle.AvgLinePen.Color), horizontalPosition, (float)verticalPosition);
            }
            
            DrawLine(e.Graphics);
        }

        protected void DrawLine(Graphics g)
        {
            visibleValues = Math.Min(this.Width / valueSpacing, drawValues.Count);

            // Dirty little "trick": initialize the first previous Point outside the bounds
            Point previousPoint = new Point(Width + valueSpacing, Height);
            Point currentPoint = new Point();

            // Draw Border on top
            ControlPaint.DrawBorder3D(g, 0, 0, Width, Height, b3dstyle);

            // Connect all visible values with lines
            for (int i = 0; i < visibleValues; i++)
            {
                currentPoint.X = previousPoint.X - valueSpacing;
                currentPoint.Y = GetVerticalPosition(drawValues[i]);

                // Actually draw the line
                g.DrawLine(perfChartStyle.ChartLinePen.Pen, previousPoint, currentPoint);

                previousPoint = currentPoint;
            }
        }

        protected int GetVerticalPosition(decimal value)
        {
            decimal result = Decimal.Zero;

            result = (MAX > 0) ? (value * (this.Height - 2) / MAX) : 0;
            result = this.Height - result - 1;
            result = Math.Round(result);
            
            return Convert.ToInt32(result);
        }
    }
}
