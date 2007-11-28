Public Class IniFile
    Protected Shared dataDir As String

    Public Shared ReadOnly Property DataDirectory() As String
        Get
            Return dataDir
        End Get
    End Property

    Public Shared Sub ReadFile(ByVal fileName As String)
        Dim xr As New Xml.XmlTextReader(fileName)
        Dim curNode, curValue As String
        While xr.Read
            If xr.NodeType = Xml.XmlNodeType.Element Then
                curNode = xr.Name
            ElseIf xr.NodeType = Xml.XmlNodeType.Text Then
                curValue = xr.Value
                Select Case curNode
                    Case "Data_Directory"
                        dataDir = curValue
                        If Not dataDir.EndsWith("\") Then dataDir &= "\"
                End Select
            End If
        End While
        xr.Close()
    End Sub

End Class
