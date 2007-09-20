using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using INIEditor.BackEnd;
using INIEditor.BackEnd.INIHelper;
using System.IO;
using INIEditor.BackEnd.XML;

namespace INIEditor.GUI
{
    public partial class MainForm : Form
    {
        #region Fields
        private INIFile _iniFile = null;
        private Hashtable _iniSettingsList = null;
        #endregion
        #region CTOR
        public MainForm()
        {
            InitializeComponent();
            InitializeFiles();
            InitializeXMLSettingsFile();

            // Select the first INI - File in the combo box
            if (cmbFiles.Items.Count > 0)
            {
                cmbFiles.SelectedIndex = 0;
            }

        }
        #endregion
        #region Initialize
        private void InitializeXMLSettingsFile()
        {
            string path = Path.Combine(Constants.XML_SETTINGS_PATH, Constants.INI_EDITOR_INIT_FILE);
            Settings xmlIniSettings = Helper.LoadObjectFromXMLFile(path);

            // ------------
            // Store the sections in an hashtable
            // ------------
            // Root (INI File)
            _iniSettingsList = new Hashtable();
            _iniSettingsList.Add(Constants.INI_FILE, xmlIniSettings);

            // Sections
            foreach (Section xmlSection in xmlIniSettings.Sections)
            {
                _iniSettingsList.Add(xmlSection.Name, xmlSection);
            }
        }

        private void InitializeFiles()
        {
            DirectoryInfo ja2DirInfo = new DirectoryInfo(Constants.JA2_PATH);

            DirectoryInfo[] dataDirs = ja2DirInfo.GetDirectories();

            foreach (DirectoryInfo di in dataDirs)
            {
                // Only list the directory, if they include the "ja2_options.ini"
                FileInfo[] files = di.GetFiles(Constants.INI_FILE, SearchOption.TopDirectoryOnly);

                if (files.Length > 0)
                {
                    string iniFile = Path.Combine(di.Name, files[0].Name);
                    cmbFiles.Items.Add(iniFile);
                }
            }
        }

        private Settings GetXMLIniFile()
        {
            Settings xmlIniSettings = null;
            if (_iniSettingsList.ContainsKey(Constants.INI_FILE))
            {
                xmlIniSettings = _iniSettingsList[Constants.INI_FILE] as Settings;
            }

            if (xmlIniSettings == null)
            {
                xmlIniSettings = new Settings();
                xmlIniSettings.Description_ENG = Constants.MISSING_INI_DESCRIPTION_ENG;
                xmlIniSettings.Description_GER = Constants.MISSING_INI_DESCRIPTION_GER;
            }
            return xmlIniSettings;
        }

        private Section GetXMLSection(string section)
        {
            Section matchingXMLSection = null;

            if (_iniSettingsList.ContainsKey(section))
            {
                matchingXMLSection = _iniSettingsList[section] as Section;
            }

            if (matchingXMLSection == null)
            {
                matchingXMLSection = new Section();
                matchingXMLSection.Description_ENG = Constants.MISSING_SECTION_DESCRIPTION_ENG;
                matchingXMLSection.Description_GER = Constants.MISSING_SECTION_DESCRIPTION_GER;
            }

            return matchingXMLSection;
        }

        private Property GetXMLProperty(string section, string property)
        {
            Property matchingXMLProperty = null;

            if (_iniSettingsList.ContainsKey(section))
            {
                Section xmlSection = _iniSettingsList[section] as Section;
                foreach (Property xmlProperty in xmlSection.Properties)
                {
                    if (xmlProperty.Name == property)
                    {
                        matchingXMLProperty = xmlProperty;
                        break;
                    }
                }
            }

            if (matchingXMLProperty == null)
            {
                matchingXMLProperty = new Property();
                matchingXMLProperty.Description_ENG = Constants.MISSING_PROPERTY_DESCRIPTION_ENG;
                matchingXMLProperty.Description_GER = Constants.MISSING_PROPERTY_DESCRIPTION_GER;
                matchingXMLProperty.DataType = Constants.MISSING_DATA_TYPE;
                matchingXMLProperty.MinValue = Constants.MISSING_MIN_VALUE;
                matchingXMLProperty.MaxValue = Constants.MISSING_MAX_VALUE;
            }

            return matchingXMLProperty;
        }

        private void InitializeSectionTree()
        {
            trvSections.Nodes.Clear();

            _iniFile = new INIFile();
            string iniFilePath = Path.Combine(Constants.JA2_PATH, cmbFiles.SelectedItem.ToString());
            _iniFile.ReadFile(iniFilePath);

            _iniFile.XMLSettings = GetXMLIniFile();
             
            TreeNode iniFileNode = new TreeNode();
            iniFileNode.Name = cmbFiles.SelectedItem.ToString();
            iniFileNode.Text = cmbFiles.SelectedItem.ToString();
            iniFileNode.Tag = _iniFile;
            trvSections.Nodes.Add(iniFileNode);

            // The ini file has a reference to the tree node
            _iniFile.Tag = iniFileNode;

            // Loop through all the sections of the ini file
            foreach (INISection section in _iniFile.Sections)
            {
                section.XMLSection = GetXMLSection(section.Name);

                TreeNode sectionNode = new TreeNode();
                sectionNode.Name = section.Name;
                sectionNode.Text = section.Name;
                sectionNode.Tag = section;
                iniFileNode.Nodes.Add(sectionNode);

                // The section has a reference to the tree node
                section.Tag = sectionNode;

                // Loop through all the properties of the current selection
                foreach (INIProperty property in section.Properties)
                {
                    property.XMLProperty = GetXMLProperty(section.Name, property.Name);

                    TreeNode propertyNode = new TreeNode();
                    propertyNode.Name = property.Name;
                    propertyNode.Text = property.Name;
                    propertyNode.Tag = property;
                    sectionNode.Nodes.Add(propertyNode);   

                    // The property has a reference to the tree node
                    property.Tag = propertyNode;
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
                tpSection.Text = Constants.TP_SECTIONS_INI_FILE;
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
                        // TODO: Language specific description
                        dgvProperties[colDescription.Index, rowIndex].Value = property.XMLProperty.Description_ENG;

                        dgvProperties[colSection.Index, rowIndex].Tag = section;
                        dgvProperties[colProperty.Index, rowIndex].Tag = property;
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
                tpSection.Text = Constants.TP_SECTIONS_SECTION;
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
                    // TODO: Language specific description
                    dgvProperties[colDescription.Index, rowIndex].Value = property.XMLProperty.Description_ENG;

                    dgvProperties[colSection.Index, rowIndex].Tag = section;
                    dgvProperties[colProperty.Index, rowIndex].Tag = property;
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

            SelectTreeNode(item);
        }

        private void SelectTreeNode<T>(T item)
        {
            TreeNode treeNode = null;
            if (item is INIFile)
            {
                INIFile iniFile = item as INIFile;
                treeNode = iniFile.Tag as TreeNode;
            }
            else if (item is INISection)
            {
                INISection section = item as INISection;
                treeNode = section.Tag as TreeNode;
            }
            else if (item is INIProperty)
            {
                INIProperty property = item as INIProperty;
                treeNode = property.Tag as TreeNode;
            }
            
            trvSections.SelectedNode = treeNode;
            trvSections.SelectedNode.Expand();
        }
        #endregion
        #region Events
        private void cmbFiles_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (cmbFiles.SelectedItem != null)
            {
                InitializeSectionTree();
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

            InitializeTabs(property, section.Name);
        }
        #endregion

        /// <summary>
        /// This method creates a new "INIEditorInit_Output.xml" file,
        /// which contains all the INI-Settings. Missing settings in the
        /// file are set to constant values (see Constants.cs) for easy replacement.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void mnuToolsGenerateXMLFile_Click(object sender, EventArgs e)
        {
            if (_iniFile != null)
            {
                Settings generatedIniSettings = new Settings();
                generatedIniSettings = _iniFile.XMLSettings;

                foreach (INISection section in _iniFile.Sections)
                {
                    Section xmlSection = new Section();
                    xmlSection = section.XMLSection;
                    
                    foreach (INIProperty property in section.Properties)
                    {
                        Property xmlProperty = new Property();
                        xmlProperty = property.XMLProperty;
     
                        xmlSection.Properties.Add(xmlProperty);
                    }

                    generatedIniSettings.Sections.Add(xmlSection);
                }

                string path = Path.Combine(Constants.XML_SETTINGS_PATH, Constants.INI_EDITOR_INIT_FILE_OUTPUT);
                Helper.SaveObjectToXMLFile(generatedIniSettings, path);
            }
        }
    }
}