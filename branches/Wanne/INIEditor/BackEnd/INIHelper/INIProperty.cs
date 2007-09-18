namespace INIEditor.BackEnd.INIHelper
{
    public class INIProperty
    {
        private string name;
        private string value;

        public string Name
        {
            get
            {
                return this.name;
            }
            set
            {
                this.name = value;
            }
        }

        public string Value
        {
            get
            {
                return this.value;
            }
            set
            {
                this.value = value;
            }
        }
    }
}