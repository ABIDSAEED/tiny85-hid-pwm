using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using HidLibrary;

namespace tiny85_hid_pwm
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        const int vid = 0x16c0;/* = 5824 voti.nl */
        const int pid = 0x05df;/* = 1503 obdev's shared PID for HIDs */

        HidDevice device;
        byte[] txbuf = new byte[2];

        public MainWindow()
        {
            InitializeComponent();

            device = HidDevices.Enumerate(vid, pid).FirstOrDefault();
            try
            {
                device.OpenDevice();
            }
            catch
            {
                valueSlider.IsEnabled = false;
            }
        }

        void Thumb_DragCompleted(object sender, DragCompletedEventArgs e)
        {
            if (device != null && device.IsOpen)
            {
                txbuf[1] = (byte)valueSlider.Value;
                device.Write(txbuf);
            }
        }
    }
}
