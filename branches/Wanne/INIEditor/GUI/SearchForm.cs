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

        public SearchForm(MainForm mainForm, SearchParams searchParams)
        {
            _mainForm = mainForm;
            InitializeComponent();
            InitializeSearchForm(searchParams);
        }

        private void InitializeSearchForm(SearchParams searchParams)
        {
            txtFindWhat.Text = searchParams.FindWhat;
            chkSectionDescriptions.Checked = searchParams.LookInSectionDescriptions;
            chkPropertyDescriptions.Checked = searchParams.LookInPropertyDescriptions;
            chkPropertyValues.Checked = searchParams.LookInPropertyValues;
        }

        private void btnSearch_Click(object sender, EventArgs e)
        {
            SearchParams searchParams = new SearchParams(txtFindWhat.Text, chkSectionDescriptions.Checked, chkPropertyDescriptions.Checked,
                chkPropertyValues.Checked);

            _mainForm.Search(searchParams);
        }

        private void SetSearchButtonState()
        {
            if (chkSectionDescriptions.Checked == false &&
                chkPropertyDescriptions.Checked == false &&
                chkPropertyValues.Checked == false)
            {
                btnSearch.Enabled = false;
            }
            else
            {
                btnSearch.Enabled = true;
            }
        }

        private void chkSectionDescriptions_CheckedChanged(object sender, EventArgs e)
        {
            SetSearchButtonState();
        }

        private void chkPropertyDescriptions_CheckedChanged(object sender, EventArgs e)
        {
            SetSearchButtonState();
        }

        private void chkPropertyValues_CheckedChanged(object sender, EventArgs e)
        {
            SetSearchButtonState();
        }
    }
}