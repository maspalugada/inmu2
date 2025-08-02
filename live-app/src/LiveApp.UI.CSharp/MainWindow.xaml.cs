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
using System.Windows.Shapes;
using LiveAppCore;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Threading;

namespace LiveApp.UI.CSharp
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private CoreFunctions coreFunctions;
        private WriteableBitmap writeableBitmap;
        private Timer timer;

        public MainWindow()
        {
            InitializeComponent();
            coreFunctions = new CoreFunctions();
            this.Loaded += MainWindow_Loaded;
        }

        private void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            // The GStreamer version check is still useful for debugging.
            // You can leave it or remove it.
            try
            {
                var version = CoreFunctions.GetGStreamerVersion();
                MessageBox.Show(version, "GStreamer Integration Test");
            }
            catch (Exception ex)
            {
                MessageBox.Show($"An error occurred: {ex.Message}", "Error");
            }
        }

        private void StartWebcam_Click(object sender, RoutedEventArgs e)
        {
            // For now, we'll just start the pipeline.
            // The frame handling will be added later.
            coreFunctions.StartWebcam();
        }

        private void StopWebcam_Click(object sender, RoutedEventArgs e)
        {
            coreFunctions.StopWebcam();
        }
    }
}
