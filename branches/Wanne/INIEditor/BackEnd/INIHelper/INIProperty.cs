namespace INIEditor.BackEnd.INIHelper
{
    public class INIProperty
    {
        #region Fields
        private string _name;
        private string _value;
        private object _tag = null;
        #endregion
        #region Properties
        public object Tag
        {
            get
            {
                return _tag;
            }
            set
            {
                _tag = value;
            }
        }

        public string Name
        {
            get
            {
                return this._name;
            }
            set
            {
                this._name = value;
            }
        }

        public string Value
        {
            get
            {
                return this._value;
            }
            set
            {
                this._value = value;
            }
        }
        #endregion
    }
}