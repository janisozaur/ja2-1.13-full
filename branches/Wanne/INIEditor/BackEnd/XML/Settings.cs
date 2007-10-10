using System.Collections;
using System.Xml.Serialization;

namespace INIEditor.BackEnd.XML
{
    #region XML Root Element
    [XmlRoot("Settings")]
    public class Settings : BaseElements
    {
        // Elementes-List
        [XmlArray("Sections"), XmlArrayItem("Section", typeof(Section))]
        public ArrayList Sections = new ArrayList();
    }
    #endregion
    #region XML Sub Elementes
    public class Section : BaseElements
    {
        // Attributes
        [XmlAttribute("name")] 
        public string Name;

        // Elementes-List
        [XmlArray("Properties"), XmlArrayItem("Property", typeof(Property))]
        public ArrayList Properties = new ArrayList();
    }

    public class Property : BaseElements
    {
        // Attributes
        [XmlAttribute("name")]
        public string Name;

        [XmlAttribute("datatype")]
        public string DataType;

        [XmlAttribute("minvalue")]
        public int MinValue;

        [XmlAttribute("maxvalue")]
        public int MaxValue;

        [XmlAttribute("interval")] 
        public int Interval;
    }
    #endregion
}
