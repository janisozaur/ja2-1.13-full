using System;
using System.Reflection;
using System.Windows.Forms;

namespace INIEditor.GUI
{
    public partial class SplashForm : Form
    {
        #region CTOR
        public SplashForm()
        {
            InitializeComponent();
        }
        #endregion

        #region Public Methods
        public void UpdateProgressBar(int progress)
        {
            pgbSplash.Value = progress;
            Application.DoEvents();
        }

        public void UpdateLoadingText(string text)
        {
            lblProgressText.Text = text;
            Application.DoEvents();
        }
        #endregion

        #region Events
        private void SplashForm_Load(object sender, EventArgs e)
        {
            //Version.Text = System.String.Format(Version.Text, Version.Major, My.Application.Info.Version.Minor)
            string versionNumer = Assembly.GetExecutingAssembly().GetName().Version.ToString();

            Version.Text = "Version: " + versionNumer;
            Application.DoEvents();
        }
        #endregion
    }
}