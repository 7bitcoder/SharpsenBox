using System;
using System.Diagnostics;
using System.IO;

namespace ChildProcessInTheSameConsole
{
    public class Program
    {
        public static void Main(string[] args)
        {
            Directory.SetCurrentDirectory("./Updater");
            var updater = new ProcessStartInfo
            {
                FileName = "AppUpdater.exe",
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
                FileName = "LaunchBox.exe",
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
