using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Security.AccessControl;
using System.Security.Principal;
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

namespace Dlls_Install
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        String sodDllDir = "";
        bool IsNeedSketchUp = false;
        public MainWindow(StartupEventArgs e)
        {
            if (e.Args.Length > 0)
            {
                sodDllDir = e.Args[0];
            }
            else
            {
                sodDllDir = Directory.GetCurrentDirectory();
                Console.WriteLine(sodDllDir);
            }
            InitializeComponent();
        }

        protected override void OnContentRendered(EventArgs e)
        {
            base.OnContentRendered(e);
            CopyDlls(sodDllDir);
        }

        void CopyDlls(string sodDllDir)
        {
            DirectoryInfo dir = new DirectoryInfo(sodDllDir);
            if (!dir.Exists)
            {
                MessageBox.Show(sodDllDir + " does not exist!");
                return;
            }
            FileInfo[] dllFiles = dir.GetFiles().Where(file => file.Extension == ".dll").ToArray(); //return files of the c dir
            int number = dllFiles.Length;
            int current = 0;
            ProgressBar_Copying.Maximum = number - 1;
            string suDir = System.IO.Path.GetFullPath(System.IO.Path.Combine(sodDllDir, "..\\..\\..\\"));
            string sudataDir = System.IO.Path.GetFullPath(System.IO.Path.Combine(sodDllDir, "..\\data\\"));
            string confirmingMessage = string.Join(",  ", dllFiles.Select(file => file.Name).ToArray());
            confirmingMessage = "Do you want to install " + number + " library  files: \"" + confirmingMessage + "\" into \"" + suDir
                + "\" and set write permission for folder \"" + sudataDir + "\".";
            if (MessageBox.Show(confirmingMessage, "Confirm", MessageBoxButton.YesNo) == MessageBoxResult.Yes)
            {
                try
                {

                    if (current > 0)
                    {
                        TextBlock_Copying.Text = number.ToString() + " library files are going to be installed.\n";
                    }
                    foreach (FileInfo dll in dllFiles) //look for first file in files array
                    {
                        File.Copy(dll.FullName, System.IO.Path.Combine(suDir, dll.Name), true);
                        TextBlock_Copying.Text += "from:\t" + dll.FullName + "\n";
                        TextBlock_Copying.Text += "to:  \t" + System.IO.Path.Combine(suDir, dll.Name) + "\n";
                        TextBlock_Copying.Text += "-------------------------------------------------------\n";
                        ProgressBar_Copying.Value = current;
                        current++;
                    }
                    DirectorySecurity sudataDirSec = new DirectorySecurity();
                    //SID "S-1-1-0" = "Everyone"
                    //SID "S-1-5-32-545" == "Users"
                    sudataDirSec.AddAccessRule(new FileSystemAccessRule(new SecurityIdentifier(@"S-1-5-32-545"), FileSystemRights.FullControl, AccessControlType.Allow));
                    System.IO.Directory.SetAccessControl(sudataDir, sudataDirSec);

                    Label_Copying.Content += "Finished.";
                    Button_ExitRestartSU.Visibility = System.Windows.Visibility.Visible;
                    TextBlock_Copying.Text += "Finished.";
                }
                catch (Exception exception)
                {
                    MessageBox.Show(exception.Message);
                }
            }
            else
            {
                TextBlock_Copying.Text = "Installation is canceled.";
                Label_Copying.Content += "Canceled";
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            IsNeedSketchUp = true;
            this.Close();
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);
            if (IsNeedSketchUp)
            {
                RestartSketchUp();
            }
        }

        void RestartSketchUp()
        {
            try
            {
                Process firstProc = new Process();
                firstProc.StartInfo.FileName = System.IO.Path.Combine(sodDllDir, "..\\..\\..\\SketchUp.exe");
                firstProc.EnableRaisingEvents = true;
                firstProc.Start();
            }
            catch (Exception ex)
            {
                Console.WriteLine("An error occurred!!!: " + ex.Message);
                return;
            }
        }
    }
}
