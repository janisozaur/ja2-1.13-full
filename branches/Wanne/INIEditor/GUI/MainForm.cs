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

            List<INISection> sections = _iniFile.Sections;

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
                foreach (INIProperty property in section.Properties)
                {
                    TreeNode propertyNode = new TreeNode();
                    propertyNode.Name = property.Name;
                    propertyNode.Text = property.Name;
                    propertyNode.Tag = property;
                    sectionNode.Nodes.Add(propertyNode);   
                }
            }
        }

        private void InitializeTabs<T>(T item, string sectionHeader)
        {
            dgvProperties.Rows.Clear();

            // INI file item
            if (item is INIFile)
            {
                if (tabActions.TabPages.Contains(tpProperty))
                {
                    tabActions.TabPages.Remove(tpProperty);
                }
                tpSection.Text = "All Sections and Properties";
                if (!tabActions.TabPages.Contains(tpSection))
                {
                    tabActions.TabPages.Add(tpSection);
                }

                lblSectionHeader.Text = sectionHeader;

                colSection.Visible = true;

                INIFile file = item as INIFile;

                foreach (INISection section in file.Sections)
                {
                    foreach (INIProperty property in section.Properties)
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
            // Section item
            else if (item is INISection)
            {
                if (tabActions.TabPages.Contains(tpProperty))
                {
                    tabActions.TabPages.Remove(tpProperty);
                }
                tpSection.Text = "All Properties in the selected Section";
                if (!tabActions.TabPages.Contains(tpSection))
                {
                    tabActions.TabPages.Add(tpSection);
                }

                lblSectionHeader.Text = sectionHeader;

                colSection.Visible = false;

                INISection section = item as INISection;
                foreach (INIProperty property in section.Properties)
                {
                    int rowIndex = dgvProperties.Rows.Add();

                    dgvProperties[colProperty.Index, rowIndex].Value = property.Name;
                    dgvProperties[colValue.Index, rowIndex].Value = property.Value;

                    dgvProperties[colSection.Index, rowIndex].Tag = section;
                    dgvProperties[colProperty.Index, rowIndex].Tag = property;
                    // TODO: Add description from xml file here!
                }
            }
            else if (item is INIProperty)
            {
                lblSectionHeader.Text = sectionHeader;

                INIProperty property = item as INIProperty;

                if (tabActions.TabPages.Contains(tpSection))
                {
                    tabActions.TabPages.Remove(tpSection);
                }

                tpProperty.Text = property.Name;
                if (!tabActions.TabPages.Contains(tpProperty))
                {
                    tabActions.TabPages.Add(tpProperty);
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
            string sectionHeader = e.Node.Text;
            if (e.Node.Tag is INIProperty)
            {
                TreeNode sectionNode = e.Node.Parent;
                sectionHeader = sectionNode.Text;

            }

            InitializeTabs(e.Node.Tag, sectionHeader);
        }

        private void dgvProperties_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            // Get the clicked row
            DataGridViewRow row = dgvProperties.Rows[e.RowIndex];
            INISection section = (INISection)row.Cells[colSection.Index].Tag;
            INIProperty property = (INIProperty)row.Cells[colProperty.Index].Tag;

            //trvSections.Nodes

            //InitializeTabs(property, section._name);
        }
        #endregion
    }
}