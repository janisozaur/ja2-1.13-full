using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Reflection;
using System.Text;
using System.Windows.Forms;

namespace INIEditor.GUI
{
    public partial class AboutForm : Form
    {
        #region CTOR
        public AboutForm()
        {
            InitializeComponent();
        }
        #endregion

        #region Events
        private void AboutForm_Load(object sender, EventArgs e)
        {
            string versionNumer = Assembly.GetExecutingAssembly().GetName().Version.ToString();
            lblVersion.Text = "Version: " + versionNumer;
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            Close();
        }
        #endregion
    }
}