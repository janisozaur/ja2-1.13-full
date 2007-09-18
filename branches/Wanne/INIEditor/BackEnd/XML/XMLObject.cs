using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.Serialization;

namespace INIEditor.BackEnd.XML
{
    [XmlRoot("INISettings")]
    public class INISettings
    {
        // Elementes-List
        [XmlArray("Sections"), XmlArrayItem("Section", typeof(Section))]
        public ArrayList Sections = new ArrayList();
    }

    public class Section
    {
        // Attributes
        [XmlAttribute("name")] 
        public string Name;

        // Elementes
        [XmlElement("Description_ENG")] 
        public string Description_ENG;

        [XmlElement("Description_GER")]
        public string Description_GER;

        // Elementes-List
        [XmlArray("Properties"), XmlArrayItem("Property", typeof(Property))]
        public ArrayList Properties = new ArrayList();
    }

    public class Property
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

        // Elementes
        [XmlElement("Description_ENG")]
        public string Description_ENG;

        [XmlElement("Description_GER")]
        public string Description_GER;
    }
}
