using System.Windows;
using LiveAppCore;

namespace LiveApp.UI.CSharp
{
    public partial class AddFilterDialog : Window
    {
        public CoreFunctions.FilterType SelectedFilter { get; private set; }

        public AddFilterDialog()
        {
            InitializeComponent();
        }

        private void OK_Click(object sender, RoutedEventArgs e)
        {
            switch (FilterComboBox.Text)
            {
                case "Grayscale":
                    SelectedFilter = CoreFunctions.FilterType.Grayscale;
                    break;
            }
            DialogResult = true;
        }

        private void Cancel_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
        }
    }
}
