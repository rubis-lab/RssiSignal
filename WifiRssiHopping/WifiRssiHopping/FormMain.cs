using NativeWifi;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WifiRssiHopping
{
    public partial class FormMain : Form
    {
        Timer timer = new Timer();

        public FormMain()
        {
            InitializeComponent();
            timer.Interval = 500;
            timer.Tick += timer_Tick;
        }

        void timer_Tick(object sender, EventArgs e)
        {
            PlotChart();
        }

        string GetStringForSSID(Wlan.Dot11Ssid ssid)
        {
            return Encoding.ASCII.GetString(ssid.SSID, 0, (int)ssid.SSIDLength);
        }

        private void buttonScan_Click(object sender, EventArgs e)
        {
            StringBuilder sb = new StringBuilder();

            try
            {
                WlanClient client = new WlanClient();
                foreach (WlanClient.WlanInterface wlanIface in client.Interfaces)
                {
                    Wlan.WlanAvailableNetwork[] networks = wlanIface.GetAvailableNetworkList(0);
                    foreach (Wlan.WlanAvailableNetwork network in networks)
                    {
                        sb.AppendFormat("SSID {0} : signal quality {1}.\r\n", GetStringForSSID(network.dot11Ssid), network.wlanSignalQuality);
                    }
                }                
            }
            catch (Win32Exception ee)
            {

            }

            textBoxOutput.Text = sb.ToString();
        }

        private void buttonStart_Click(object sender, EventArgs e)
        {
            timer.Start();
        }

        Random rand = new Random(0);
        private void PlotChart()
        {
            if (chart == null)
                return;

            int maxValue = 0;

            chart.AddValue(rand.Next(100));
        }
    }
}
