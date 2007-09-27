using System;
using System.Collections.Generic;
using System.Text;

namespace INIEditor.BackEnd
{
    public class SearchParams
    {
        private string _findWhat = "";
        private bool _lookInSectionDescriptions = false;
        private bool _lookInPropertyDescriptions = true;
        private bool _lookInPropertyValues = true;

        #region CTOR
        public SearchParams()
        {
        }

        public SearchParams(string findWhat, bool lookInSectionDescriptions,
            bool lookInPropertyDescriptions, bool lookInPropertyValues)
        {
            _findWhat = findWhat;
            _lookInSectionDescriptions = lookInSectionDescriptions;
            _lookInPropertyDescriptions = lookInPropertyDescriptions;
            _lookInPropertyValues = lookInPropertyValues;
        }
        #endregion

        public string FindWhat
        {
            get { return _findWhat; }
        }

        public bool LookInSectionDescriptions
        {
            get { return _lookInSectionDescriptions; }
        }

        public bool LookInPropertyDescriptions
        {
            get { return _lookInPropertyDescriptions; }
        }

        public bool LookInPropertyValues
        {
            get { return _lookInPropertyValues; }
        }
    }
}
