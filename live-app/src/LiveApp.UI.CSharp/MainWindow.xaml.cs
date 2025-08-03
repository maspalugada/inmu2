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
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using LiveAppCore;

namespace LiveApp.UI.CSharp
{
    public class Source
    {
        public string Id { get; set; }
        public string Name { get; set; }
    }

    public class Scene
    {
        public string Name { get; set; }
        public ObservableCollection<Source> Sources { get; set; } = new ObservableCollection<Source>();
    }

    public partial class MainWindow : Window
    {
        private CoreFunctions coreFunctions;
        private Dictionary<string, WriteableBitmap> writeableBitmaps = new Dictionary<string, WriteableBitmap>();
        private string previewId;
        private string programId;
        private bool isRendering = false;

        private ObservableCollection<Scene> scenes = new ObservableCollection<Scene>();

        public MainWindow()
        {
            InitializeComponent();
            coreFunctions = new CoreFunctions();
            coreFunctions.OnFrameReady += OnFrameReady;
            this.Closing += (s, e) =>
            {
                foreach (var id in writeableBitmaps.Keys)
                {
                    coreFunctions.StopSource(id);
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

            if (!writeableBitmaps.ContainsKey(id) || writeableBitmaps[id].Width != width || writeableBitmaps[id].Height != height)
            {
                writeableBitmaps[id] = new WriteableBitmap(width, height, 96, 96, PixelFormats.Bgr32, null);
            }

            var bitmap = writeableBitmaps[id];
            bitmap.Lock();
            System.Runtime.InteropServices.Marshal.Copy(frameData, 0, bitmap.BackBuffer, frameData.Length);
            bitmap.AddDirtyRect(new Int32Rect(0, 0, width, height));
            bitmap.Unlock();

            if (id == previewId)
            {
                PreviewImage.Source = bitmap;
            }
            if (id == programId)
            {
                ProgramImage.Source = bitmap;
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
                    writeableBitmaps.Remove(source.Id);
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
                writeableBitmaps.Remove(selectedSource.Id);
                selectedScene.Sources.Remove(selectedSource);
            }
        }

        private void ScenesListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (ScenesListBox.SelectedItem is Scene selectedScene)
            {
                SourcesListBox.ItemsSource = selectedScene.Sources;
            }
        }

        private void SourcesListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (SourcesListBox.SelectedItem is Source selectedSource)
            {
                coreFunctions.SetAsPreview(selectedSource.Id);
                previewId = selectedSource.Id;
            }
        }

        private void Transition_Click(object sender, RoutedEventArgs e)
        {
            coreFunctions.Transition();
            programId = previewId;
            if (previewId != null)
            {
                ProgramImage.Source = writeableBitmaps.ContainsKey(previewId) ? writeableBitmaps[previewId] : null;
            }
        }

        private bool isDragging = false;
        private Point dragStartPoint;

        private void PreviewImage_MouseLeftButtonDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (SourcesListBox.SelectedItem is Source selectedSource)
            {
                isDragging = true;
                dragStartPoint = e.GetPosition(PreviewImage);
                PreviewImage.CaptureMouse();
            }
        }

        private void PreviewImage_MouseMove(object sender, System.Windows.Input.MouseEventArgs e)
        {
            if (isDragging)
            {
                Point currentPoint = e.GetPosition(PreviewImage);
                double deltaX = currentPoint.X - dragStartPoint.X;
                double deltaY = currentPoint.Y - dragStartPoint.Y;

                // This is where we would update the position of the source in the C++ core.
                // For now, we will just move the image control.
                Canvas.SetLeft(PreviewImage, Canvas.GetLeft(PreviewImage) + deltaX);
                Canvas.SetTop(PreviewImage, Canvas.GetTop(PreviewImage) + deltaY);

                dragStartPoint = currentPoint;
            }
        }

        private void PreviewImage_MouseLeftButtonUp(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            isDragging = false;
            PreviewImage.ReleaseMouseCapture();
        }
    }
}
