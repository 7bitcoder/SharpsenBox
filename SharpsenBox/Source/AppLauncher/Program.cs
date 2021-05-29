using System;
using System.Diagnostics;
using System.IO;

namespace ChildProcessInTheSameConsole
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var processName = System.IO.Path.GetFileNameWithoutExtension(System.Reflection.Assembly.GetEntryAssembly().Location);
            // if instance of program is already running return;
            if (System.Diagnostics.Process.GetProcessesByName(processName).Count() > 1) 
                return;
                
            Directory.SetCurrentDirectory("./Updater");
            var updater = new ProcessStartInfo
            {
                FileName = "Updater.exe",
                Arguments = ""
            };

            Console.WriteLine("Starting updater");
            using (var process = Process.Start(updater))
            {
                process.WaitForExit();
            }

            Directory.SetCurrentDirectory("../bin");
            var app = new ProcessStartInfo
            {
                FileName = "SharpsenBox.exe",
                Arguments = ""
            };

            Console.WriteLine("Starting aplication");
            using (var process = Process.Start(app))
            {
                process.WaitForExit();
            }
        }
    }
}
