using System;
using System.Windows.Forms;
using INIEditor.BackEnd;
using INIEditor.GUI;

namespace INIEditor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            try
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);

                MainForm mainForm = new MainForm();
                Application.Run(mainForm);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, Constants.APP_NAME, MessageBoxButtons.OK, MessageBoxIcon.Error);  
            }            
        }
    }
}