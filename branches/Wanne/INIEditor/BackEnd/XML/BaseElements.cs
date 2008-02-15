using System.Xml.Serialization;

namespace INIEditor.BackEnd.XML
{
    public class BaseElements
    {
        // Elementes
        [XmlElement("Description_ENG")]
        public string Description_ENG;

        [XmlElement("Description_GER")]
        public string Description_GER;

        [XmlElement("Description_RUS")] 
        public string Description_RUS;
    }
}
