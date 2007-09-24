using INIEditor.BackEnd.XML;

namespace INIEditor.BackEnd.INIHelper
{
    public class INIProperty
    {
        #region Fields
        private string _name;
        private string _currentValue;   // The current value from the ini file
        private string _newValue;       // The new value set by the INI-Editor (from the GUI)
        private INISection _section;    // The section the property belongs to
        private object _tag = null;
        private Property _xmlProperty = null;
        #endregion
        #region Properties
        public object Tag
        {
            get { return _tag; }
            set { _tag = value; }
        }

        public string Name
        {
            get { return this._name; }
            set { this._name = value; }
        }

        public string CurrentValue
        {
            get { return this._currentValue; }
            set { this._currentValue = value; }
        }

        public string NewValue
        {
            get { return _newValue; }
            set { _newValue = value; }
        }

        public INISection Section
        {
            get { return _section; }
            set { _section = value; }
        }

        public Property XMLProperty
        {
            get { return _xmlProperty; }
            set { _xmlProperty = value; }
        }
        #endregion
    }
}