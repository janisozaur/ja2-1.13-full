using System;
using System.Collections.Generic;
using System.Text;

namespace INIEditor.BackEnd
{
    public class Constants
    {
        public const string APP_NAME = "JA2 1.13 - INI Editor";

        // TODO: This is for test version!
        //public const string JA2_PATH = @"C:\games\jagged alliance 2";
        //public const string XML_SETTINGS_PATH = @"C:\games\jagged alliance 2";

        // TODO: This is for release version!
        public const string JA2_PATH = @".\";
        public const string XML_SETTINGS_PATH = @".\";

        public const string INI_FILE = "ja2_options.ini";
        public const string INI_FILE_OUT = "ja2_options.ini";   // INI Output file
        public const string INI_EDITOR_INIT_FILE = "INIEditorInit.xml";
        public const string INI_EDITOR_INIT_FILE_OUTPUT = "INIEditorInit.xml";   // XML Output file

        public const string TP_SECTIONS_INI_FILE = "All Sections and Properties";
        public const string TP_SECTIONS_SECTION = "All Properties in the selected Section";

        

        public const string MISSING_INI_DESCRIPTION_ENG = "MISSING: INI DESCRIPTION";
        public const string MISSING_INI_DESCRIPTION_GER = "FEHLEND: INI BESCHREIBUNG";
        public const string MISSING_SECTION_DESCRIPTION_ENG = "MISSING: SECTION DESCRIPTION";
        public const string MISSING_SECTION_DESCRIPTION_GER = "FEHLEND: SEKTION BESCHREIBUNG";
        public const string MISSING_PROPERTY_DESCRIPTION_ENG = "MISSING: PROPERTY DESCRIPTION";
        public const string MISSING_PROPERTY_DESCRIPTION_GER = "FEHLEND: PROPERTY BESCHREIBUNG";
        public const string MISSING_DATA_TYPE = "MISSING_DATA_TYPE";
        public const int MISSING_MIN_VALUE = -99999;
        public const int MISSING_MAX_VALUE = 99999;
        public const string MISSING_PROPERTY_VALUE = "66666"; // Use a string type, because we can cast it with to any needed type!
        public const int PROPERTY_INVERVAL = 1;

        public static readonly System.Drawing.Color CHANGED_TREE_NODE_TEXT_COLOR = System.Drawing.Color.Red;
        public static readonly System.Drawing.Color DEFAULT_TREE_NODE_TEXT_COLOR = System.Drawing.Color.Black;

        public static readonly System.Drawing.Color DEFAULT_SEARCH_RESULT_TEXT_COLOR = System.Drawing.Color.Blue;
    }
}
