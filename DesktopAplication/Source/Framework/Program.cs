using System;
using System.Diagnostics;

namespace ChildProcessInTheSameConsole {
    public class Program {
        public static void Main(string[] args) {
            var processInfo = new ProcessStartInfo {
                FileName = "./bin/LaunchBox.exe",
                Arguments = ""
            };

            Console.WriteLine("Starting child process...");
            using(var process = Process.Start(processInfo)) {
                process.WaitForExit();
            }
        }
    }
}