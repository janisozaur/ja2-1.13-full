using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using INIEditor.BackEnd;

namespace INIEditor.GUI
{
    public partial class SearchForm : Form
    {
        private MainForm _mainForm;
        private Enumerations.Language _language;

        public SearchForm(MainForm mainForm, Enumerations.Language language)
        {
            _mainForm = mainForm;
            InitializeComponent();
            cmbDescLanguage.SelectedIndex = 0;
        }

        private void btnSearch_Click(object sender, EventArgs e)
        {
            SearchParams searchParams = new SearchParams(txtFindWhat.Text, chkSectionDescriptions.Checked, chkPropertyDescriptions.Checked,
                chkPropertyValues.Checked, _language);

            _mainForm.Search(searchParams);
        }
    }
}