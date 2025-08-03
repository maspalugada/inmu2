using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using LiveAppCore;

namespace LiveApp.UI.CSharp
{
    public partial class MainWindow : Window
    {
        private CoreFunctions coreFunctions;
        private WriteableBitmap writeableBitmap;
        private bool isRendering = false;

        public MainWindow()
        {
            InitializeComponent();
            coreFunctions = new CoreFunctions();
            coreFunctions.OnFrameReady += OnFrameReady;
            this.Closing += (s, e) => coreFunctions.StopWebcam();
        }

        private void OnFrameReady()
        {
            if (!isRendering)
            {
                isRendering = true;
                Dispatcher.BeginInvoke(new Action(() =>
                {
                    RenderFrame();
                    isRendering = false;
                }));
            }
        }

        private void RenderFrame()
        {
            int width = 0, height = 0;
            var frameData = coreFunctions.GetLatestFrame(ref width, ref height);

            if (frameData == null || width == 0 || height == 0)
                return;

            if (writeableBitmap == null || writeableBitmap.Width != width || writeableBitmap.Height != height)
            {
                writeableBitmap = new WriteableBitmap(width, height, 96, 96, PixelFormats.Bgr32, null);
                WebcamImage.Source = writeableBitmap;
            }

            writeableBitmap.Lock();
            System.Runtime.InteropServices.Marshal.Copy(frameData, 0, writeableBitmap.BackBuffer, frameData.Length);
            writeableBitmap.AddDirtyRect(new Int32Rect(0, 0, width, height));
            writeableBitmap.Unlock();
        }

        private void StartWebcam_Click(object sender, RoutedEventArgs e)
        {
            coreFunctions.StartWebcam();
        }

        private void StartScreenCapture_Click(object sender, RoutedEventArgs e)
        {
            coreFunctions.StartScreenCapture();
        }

        private void Stop_Click(object sender, RoutedEventArgs e)
        {
            coreFunctions.Stop();
            writeableBitmap = null;
            WebcamImage.Source = null;
        }
    }
}
