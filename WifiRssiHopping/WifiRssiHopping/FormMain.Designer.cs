namespace WifiRssiHopping
{
    partial class FormMain
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            SpPerfChart.ChartPen chartPen1 = new SpPerfChart.ChartPen();
            SpPerfChart.ChartPen chartPen2 = new SpPerfChart.ChartPen();
            SpPerfChart.ChartPen chartPen3 = new SpPerfChart.ChartPen();
            this.textBoxOutput = new System.Windows.Forms.TextBox();
            this.buttonScan = new System.Windows.Forms.Button();
            this.buttonStart = new System.Windows.Forms.Button();
            this.chart = new WifiRssiHopping.TaskChart();
            this.SuspendLayout();
            // 
            // textBoxOutput
            // 
            this.textBoxOutput.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxOutput.Location = new System.Drawing.Point(12, 12);
            this.textBoxOutput.Multiline = true;
            this.textBoxOutput.Name = "textBoxOutput";
            this.textBoxOutput.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxOutput.Size = new System.Drawing.Size(309, 290);
            this.textBoxOutput.TabIndex = 0;
            // 
            // buttonScan
            // 
            this.buttonScan.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.buttonScan.Location = new System.Drawing.Point(12, 308);
            this.buttonScan.Name = "buttonScan";
            this.buttonScan.Size = new System.Drawing.Size(75, 23);
            this.buttonScan.TabIndex = 1;
            this.buttonScan.Text = "Scan";
            this.buttonScan.UseVisualStyleBackColor = true;
            this.buttonScan.Click += new System.EventHandler(this.buttonScan_Click);
            // 
            // buttonStart
            // 
            this.buttonStart.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonStart.Location = new System.Drawing.Point(544, 308);
            this.buttonStart.Name = "buttonStart";
            this.buttonStart.Size = new System.Drawing.Size(75, 23);
            this.buttonStart.TabIndex = 3;
            this.buttonStart.Text = "Start";
            this.buttonStart.UseVisualStyleBackColor = true;
            this.buttonStart.Click += new System.EventHandler(this.buttonStart_Click);
            // 
            // chart
            // 
            this.chart.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.chart.BorderStyle = System.Windows.Forms.Border3DStyle.Flat;
            this.chart.DeadlineMiss = false;
            this.chart.Font = new System.Drawing.Font("맑은 고딕", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.World);
            this.chart.LastUpdate = new System.DateTime(((long)(0)));
            this.chart.Location = new System.Drawing.Point(327, 12);
            this.chart.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.chart.Name = "chart";
            this.chart.PerfChartStyle.AntiAliasing = true;
            chartPen1.Color = System.Drawing.Color.HotPink;
            chartPen1.DashStyle = System.Drawing.Drawing2D.DashStyle.Solid;
            chartPen1.Width = 1F;
            this.chart.PerfChartStyle.AvgLinePen = chartPen1;
            this.chart.PerfChartStyle.BackgroundColorBottom = System.Drawing.Color.Pink;
            this.chart.PerfChartStyle.BackgroundColorTop = System.Drawing.Color.LavenderBlush;
            chartPen2.Color = System.Drawing.Color.Black;
            chartPen2.DashStyle = System.Drawing.Drawing2D.DashStyle.Solid;
            chartPen2.Width = 1F;
            this.chart.PerfChartStyle.ChartLinePen = chartPen2;
            chartPen3.Color = System.Drawing.Color.Silver;
            chartPen3.DashStyle = System.Drawing.Drawing2D.DashStyle.Dot;
            chartPen3.Width = 1F;
            this.chart.PerfChartStyle.HorizontalGridPen = chartPen3;
            this.chart.PerfChartStyle.ShowAverageLine = false;
            this.chart.PerfChartStyle.ShowCurrentMaxValue = false;
            this.chart.PerfChartStyle.ShowHorizontalGridLines = true;
            this.chart.PerfChartStyle.ShowVerticalGridLines = true;
            this.chart.PerfChartStyle.VerticalGridPen = chartPen3;
            this.chart.ScaleMode = SpPerfChart.ScaleMode.Relative;
            this.chart.Size = new System.Drawing.Size(292, 289);
            this.chart.TabIndex = 2;
            this.chart.TimerInterval = 100;
            this.chart.TimerMode = SpPerfChart.TimerMode.Disabled;
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(631, 343);
            this.Controls.Add(this.buttonStart);
            this.Controls.Add(this.chart);
            this.Controls.Add(this.buttonScan);
            this.Controls.Add(this.textBoxOutput);
            this.MinimumSize = new System.Drawing.Size(640, 380);
            this.Name = "FormMain";
            this.Text = "WiFi RSSI";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBoxOutput;
        private System.Windows.Forms.Button buttonScan;
        private TaskChart chart;
        private System.Windows.Forms.Button buttonStart;
    }
}

