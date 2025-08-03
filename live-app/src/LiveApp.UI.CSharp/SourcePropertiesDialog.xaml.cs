using System;
using System.Collections.Generic;
using System.Windows;
using LiveAppCore;

namespace LiveApp.UI.CSharp
{
    public partial class SourcePropertiesDialog : Window
    {
        private CoreFunctions coreFunctions;
        public string SelectedDevice { get; private set; }
        public string SelectedCapability { get; private set; }

        public SourcePropertiesDialog(CoreFunctions core)
        {
            InitializeComponent();
            coreFunctions = core;
            PopulateDeviceComboBox();
        }

        private void PopulateDeviceComboBox()
        {
            var devices = coreFunctions.GetVideoDevices();
            DeviceComboBox.ItemsSource = devices;
        }

        private void DeviceComboBox_SelectionChanged(object sender, System.Windows.Controls.SelectionChangedEventArgs e)
        {
            if (DeviceComboBox.SelectedItem is string selectedDevice)
            {
                var capabilities = coreFunctions.GetDeviceCapabilities(selectedDevice);
                CapabilitiesComboBox.ItemsSource = capabilities;
            }
        }

        private void OK_Click(object sender, RoutedEventArgs e)
        {
            SelectedDevice = DeviceComboBox.SelectedItem as string;
            SelectedCapability = CapabilitiesComboBox.SelectedItem as string;
            DialogResult = true;
        }

        private void Cancel_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
        }
    }
}
