using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using LiveAppCore;

namespace LiveApp.UI.CSharp
{
    public class Source : INotifyPropertyChanged
    {
        public string Id { get; set; }
        public string Name { get; set; }
        private double x;
        public double X { get { return x; } set { x = value; OnPropertyChanged(nameof(X)); } }
        private double y;
        public double Y { get { return y; } set { y = value; OnPropertyChanged(nameof(Y)); } }
        public double Width { get; set; } = 320;
        public double Height { get; set; } = 240;
        private WriteableBitmap bitmap;
        public WriteableBitmap Bitmap { get { return bitmap; } set { bitmap = value; OnPropertyChanged(nameof(Bitmap)); } }
        public double Volume { get; set; } = 1.0;
        public bool IsMuted { get; set; } = false;

        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(string name)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }
    }

    public class Scene
    {
        public string Name { get; set; }
        public ObservableCollection<Source> Sources { get; set; } = new ObservableCollection<Source>();
    }

    public partial class MainWindow : Window
    {
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using LiveAppCore;

namespace LiveApp.UI.CSharp
{
    public class Source : INotifyPropertyChanged
    {
        public string Id { get; set; }
        public string Name { get; set; }
        private double x;
        public double X { get { return x; } set { x = value; OnPropertyChanged(nameof(X)); } }
        private double y;
        public double Y { get { return y; } set { y = value; OnPropertyChanged(nameof(Y)); } }
        public double Width { get; set; } = 320;
        public double Height { get; set; } = 240;
        private WriteableBitmap bitmap;
        public WriteableBitmap Bitmap { get { return bitmap; } set { bitmap = value; OnPropertyChanged(nameof(Bitmap)); } }

        public event PropertyChangedEventHandler PropertyChanged;
        protected void OnPropertyChanged(string name)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }
    }

    public class Scene
    {
        public string Name { get; set; }
        public ObservableCollection<Source> Sources { get; set; } = new ObservableCollection<Source>();
    }

    public partial class MainWindow : Window
    {
        private CoreFunctions coreFunctions;
        private WriteableBitmap programBitmap;
        private bool isRendering = false;

        private ObservableCollection<Scene> scenes = new ObservableCollection<Scene>();

        public MainWindow()
        {
            InitializeComponent();
            coreFunctions = new CoreFunctions();
            coreFunctions.OnFrameReady += OnFrameReady;
            this.Closing += (s, e) =>
            {
                foreach (var scene in scenes)
                {
                    foreach (var source in scene.Sources)
                    {
                        coreFunctions.StopSource(source.Id);
                    }
                }
            };

            ScenesListBox.ItemsSource = scenes;
            AddScene_Click(null, null);
        }

        private void OnFrameReady(string id)
        {
            if (!isRendering)
            {
                isRendering = true;
                Dispatcher.BeginInvoke(new Action(() =>
                {
                    RenderFrame(id);
                    isRendering = false;
                }));
            }
        }

        private void RenderFrame(string id)
        {
            int width = 0, height = 0;
            var frameData = coreFunctions.GetLatestFrame(id, ref width, ref height);

            if (frameData == null || width == 0 || height == 0)
                return;

            if (id == "program")
            {
                if (programBitmap == null || programBitmap.Width != width || programBitmap.Height != height)
                {
                    programBitmap = new WriteableBitmap(width, height, 96, 96, PixelFormats.Bgr32, null);
                    ProgramImage.Source = programBitmap;
                }

                programBitmap.Lock();
                System.Runtime.InteropServices.Marshal.Copy(frameData, 0, programBitmap.BackBuffer, frameData.Length);
                programBitmap.AddDirtyRect(new Int32Rect(0, 0, width, height));
                programBitmap.Unlock();
            }
            else
            {
                Source sourceToUpdate = null;
                foreach (var scene in scenes)
                {
                    foreach (var source in scene.Sources)
                    {
                        if (source.Id == id)
                        {
                            sourceToUpdate = source;
                            break;
                        }
                    }
                    if (sourceToUpdate != null) break;
                }

                if (sourceToUpdate != null)
                {
                    if (sourceToUpdate.Bitmap == null || sourceToUpdate.Bitmap.Width != width || sourceToUpdate.Bitmap.Height != height)
                    {
                        sourceToUpdate.Bitmap = new WriteableBitmap(width, height, 96, 96, PixelFormats.Bgr32, null);
                    }

                    sourceToUpdate.Bitmap.Lock();
                    System.Runtime.InteropServices.Marshal.Copy(frameData, 0, sourceToUpdate.Bitmap.BackBuffer, frameData.Length);
                    sourceToUpdate.Bitmap.AddDirtyRect(new Int32Rect(0, 0, width, height));
                    sourceToUpdate.Bitmap.Unlock();
                }
            }
        }

        private void AddScene_Click(object sender, RoutedEventArgs e)
        {
            scenes.Add(new Scene { Name = "Scene " + (scenes.Count + 1) });
            ScenesListBox.SelectedIndex = scenes.Count - 1;
        }

        private void RemoveScene_Click(object sender, RoutedEventArgs e)
        {
            if (ScenesListBox.SelectedItem is Scene selectedScene)
            {
                foreach (var source in selectedScene.Sources)
                {
                    coreFunctions.StopSource(source.Id);
                }
                scenes.Remove(selectedScene);
            }
        }

        private void AddWebcam_Click(object sender, RoutedEventArgs e)
        {
            if (ScenesListBox.SelectedItem is Scene selectedScene)
            {
                string id = coreFunctions.CreateWebcamSource();
                if (id != null)
                {
                    selectedScene.Sources.Add(new Source { Id = id, Name = "Webcam" });
                }
            }
        }

        private void AddScreenCapture_Click(object sender, RoutedEventArgs e)
        {
            if (ScenesListBox.SelectedItem is Scene selectedScene)
            {
                string id = coreFunctions.CreateScreenCaptureSource();
                if (id != null)
                {
                    selectedScene.Sources.Add(new Source { Id = id, Name = "Screen Capture" });
                }
            }
        }

        private void AddVideoFile_Click(object sender, RoutedEventArgs e)
        {
            if (ScenesListBox.SelectedItem is Scene selectedScene)
            {
                var openFileDialog = new Microsoft.Win32.OpenFileDialog();
                openFileDialog.Filter = "Video Files (*.mp4, *.mov, *.avi)|*.mp4;*.mov;*.avi|All files (*.*)|*.*";
                if (openFileDialog.ShowDialog() == true)
                {
                    string id = coreFunctions.CreateVideoFileSource(openFileDialog.FileName);
                    if (id != null)
                    {
                        selectedScene.Sources.Add(new Source { Id = id, Name = System.IO.Path.GetFileName(openFileDialog.FileName) });
                    }
                }
            }
        }

        private void RemoveSource_Click(object sender, RoutedEventArgs e)
        {
            if (ScenesListBox.SelectedItem is Scene selectedScene && SourcesListBox.SelectedItem is Source selectedSource)
            {
                coreFunctions.StopSource(selectedSource.Id);
                selectedScene.Sources.Remove(selectedSource);
            }
        }

        private void ScenesListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (ScenesListBox.SelectedItem is Scene selectedScene)
            {
                PreviewItemsControl.ItemsSource = selectedScene.Sources;
                AudioMixerListBox.ItemsSource = selectedScene.Sources;
            }
        }

        private void Volume_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if ((sender as FrameworkElement).DataContext is Source source)
            {
                coreFunctions.SetSourceVolume(source.Id, e.NewValue);
            }
        }

        private void Mute_Checked(object sender, RoutedEventArgs e)
        {
            if ((sender as FrameworkElement).DataContext is Source source)
            {
                coreFunctions.SetSourceMute(source.Id, true);
            }
        }

        private void Mute_Unchecked(object sender, RoutedEventArgs e)
        {
            if ((sender as FrameworkElement).DataContext is Source source)
            {
                coreFunctions.SetSourceMute(source.Id, false);
            }
        }

        private void SourcesListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            // This is now handled by the ItemsControl
        }

        private void Transition_Click(object sender, RoutedEventArgs e)
        {
            coreFunctions.Transition();
        }

        private bool isDragging = false;
        private Point dragStartPoint;
        private Source draggedSource;

        private void PreviewImage_MouseLeftButtonDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if ((sender as FrameworkElement).DataContext is Source selectedSource)
            {
                isDragging = true;
                draggedSource = selectedSource;
                dragStartPoint = e.GetPosition(this);
                (sender as UIElement).CaptureMouse();
            }
        }

        private void PreviewImage_MouseMove(object sender, System.Windows.Input.MouseEventArgs e)
        {
            if (isDragging && draggedSource != null)
            {
                Point currentPoint = e.GetPosition(this);
                double deltaX = currentPoint.X - dragStartPoint.X;
                double deltaY = currentPoint.Y - dragStartPoint.Y;

                draggedSource.X += deltaX;
                draggedSource.Y += deltaY;

                coreFunctions.UpdateSourcePosition(draggedSource.Id, (int)draggedSource.X, (int)draggedSource.Y);

                dragStartPoint = currentPoint;
            }
        }

        private void PreviewImage_MouseLeftButtonUp(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            isDragging = false;
            draggedSource = null;
            (sender as UIElement).ReleaseMouseCapture();
        }
    }
}
