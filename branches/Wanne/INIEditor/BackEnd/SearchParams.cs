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
        private bool _lookInProperties = true;

        #region CTOR
        public SearchParams()
        {
        }

        public SearchParams(string findWhat, bool lookInSectionDescriptions,
            bool lookInPropertyDescriptions, bool lookInPropertyValues, bool lookInProperties)
        {
            _findWhat = findWhat;
            _lookInSectionDescriptions = lookInSectionDescriptions;
            _lookInPropertyDescriptions = lookInPropertyDescriptions;
            _lookInPropertyValues = lookInPropertyValues;
            _lookInProperties = lookInProperties;
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

        public bool LookInProperties
        {
            get { return _lookInProperties; }
        }
    }
}
