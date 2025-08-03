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

        private string sourceId;

        public SourcePropertiesDialog(CoreFunctions core, string id)
        {
            InitializeComponent();
            coreFunctions = core;
            sourceId = id;
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

            // This is a simplified implementation. A real implementation would
            // keep track of the filters that have been added and removed.
            foreach (var item in FiltersListBox.Items)
            {
                if (item is CoreFunctions.FilterType filterType)
                {
                    coreFunctions.AddFilter(sourceId, filterType);
                }
            }

            DialogResult = true;
        }

        private void Cancel_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
        }

        private void AddFilter_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new AddFilterDialog();
            if (dialog.ShowDialog() == true)
            {
                FiltersListBox.Items.Add(dialog.SelectedFilter);
            }
        }

        private void RemoveFilter_Click(object sender, RoutedEventArgs e)
        {
            if (FiltersListBox.SelectedItem != null)
            {
                FiltersListBox.Items.Remove(FiltersListBox.SelectedItem);
            }
        }
    }
}
