using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using INIEditor.BackEnd.INIHelper;
using System.IO;

namespace INIEditor.GUI
{
    public partial class MainForm : Form
    {
        #region Constants
        private const string JA2_PATH = @"C:\games\jagged alliance 2";
        #endregion
        #region Fields
        private INIFile _iniFile = null;
        #endregion
        #region CTOR
        public MainForm()
        {
            InitializeComponent();
            InitializeFiles();
            //InitializeTabs();
        }
        #endregion
        #region Initialize
        private void InitializeFiles()
        {
            DirectoryInfo ja2DirInfo = new DirectoryInfo(JA2_PATH);

            DirectoryInfo[] dataDirs = ja2DirInfo.GetDirectories();

            foreach (DirectoryInfo di in dataDirs)
            {
                // Only list the directory, if they include the "ja2_options.ini"
                FileInfo[] files = di.GetFiles("ja2_options.ini", SearchOption.TopDirectoryOnly);

                if (files.Length > 0)
                {
                    string iniFile = Path.Combine(di.Name, files[0].Name);
                    cmbFiles.Items.Add(iniFile);
                }
            }

            if (cmbFiles.Items.Count > 0)
            {
                cmbFiles.SelectedIndex = 0;    
            }
        }

        private void InitializeSections()
        {
            trvSections.Nodes.Clear();

            _iniFile = new INIFile();
            string iniFilePath = Path.Combine(JA2_PATH, cmbFiles.SelectedItem.ToString());
            _iniFile.ReadFile(iniFilePath);

            List<INISection> sections = _iniFile.GetSections();

            TreeNode iniFileNode = new TreeNode();
            iniFileNode.Name = cmbFiles.SelectedItem.ToString();
            iniFileNode.Text = cmbFiles.SelectedItem.ToString();
            iniFileNode.Tag = _iniFile;
            trvSections.Nodes.Add(iniFileNode);

            // Loop through all the sections of the ini file
            foreach (INISection section in sections)
            {
                TreeNode sectionNode = new TreeNode();
                sectionNode.Name = section.Name;
                sectionNode.Text = section.Name;
                sectionNode.Tag = section;
                iniFileNode.Nodes.Add(sectionNode);

                // Loop through all the properties of the current selection
                List<INIProperty> properties = section.GetPropertys();
                foreach (INIProperty property in properties)
                {
                    TreeNode propertyNode = new TreeNode();
                    propertyNode.Name = property.Name;
                    propertyNode.Text = property.Name;
                    propertyNode.Tag = property;
                    sectionNode.Nodes.Add(propertyNode);   
                }
            }
        }

        private void InitializeTabs<T>(T node, string sectionHeader)
        {
            tpSection.Text = sectionHeader;

            // INI file node
            if (node is INIFile)
            {
                INIFile file = node as INIFile;
            }
            else if (node is INISection)
            {
                INISection section = node as INISection;
            }
        }

        private void InitializePropertiesGrid<T>(T node, string sectionHeader)
        {
            lblSectionHeader.Text = sectionHeader;
            dgvProperties.Rows.Clear();

            // INI file node
            if (node is INIFile)
            {
                colSection.Visible = true;

                INIFile file = node as INIFile;

                List<INISection> sections = file.GetSections();
                foreach (INISection section in sections)
                {
                    List<INIProperty> properties = section.GetPropertys();
                    foreach (INIProperty property in properties)
                    {
                        int rowIndex = dgvProperties.Rows.Add();

                        dgvProperties[colSection.Index, rowIndex].Value = section.Name;
                        dgvProperties[colProperty.Index, rowIndex].Value = property.Name;
                        dgvProperties[colValue.Index, rowIndex].Value = property.Value;

                        dgvProperties[colSection.Index, rowIndex].Tag = section;
                        dgvProperties[colProperty.Index, rowIndex].Tag = property;
                        // TODO: Add description from xml file here!
                    }
                }

            }
            // Section node
            else if (node is INISection)
            {
                colSection.Visible = false;

                INISection section = node as INISection;
                List<INIProperty> properties = section.GetPropertys();
                foreach (INIProperty property in properties)
                {
                    int rowIndex = dgvProperties.Rows.Add();

                    dgvProperties[colProperty.Index, rowIndex].Value = property.Name;
                    dgvProperties[colValue.Index, rowIndex].Value = property.Value;

                    dgvProperties[colSection.Index, rowIndex].Tag = section;
                    dgvProperties[colProperty.Index, rowIndex].Tag = property;
                    // TODO: Add description from xml file here!
                }
            }
        }
        #endregion
        #region Events
        private void cmbFiles_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cmbFiles.SelectedItem != null)
            {
                InitializeSections();
                if (trvSections.Nodes.Count > 0)
                {
                    trvSections.SelectedNode = trvSections.Nodes[0];
                    tpProperty.Visible = false;
                }
            }
        }

        private void trvSections_AfterSelect(object sender, TreeViewEventArgs e)
        {
            InitializeTabs(e.Node.Tag, e.Node.Name);
            InitializePropertiesGrid(e.Node.Tag, e.Node.Name);
        }

        private void dgvProperties_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            // Get the clicked row
            DataGridViewRow row = dgvProperties.Rows[e.RowIndex];
            INISection section = (INISection)row.Cells[colSection.Index].Tag;
            INIProperty property = (INIProperty)row.Cells[colProperty.Index].Tag;

            //DetailForm detailForm = new DetailForm(section, property);
            //DialogResult result = detailForm.ShowDialog();
        }
        #endregion
    }
}