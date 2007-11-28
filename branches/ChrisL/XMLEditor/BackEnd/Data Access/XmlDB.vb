Public Class XmlDB
    Implements IDisposable

    Protected ds As DataSet
    Private disposedValue As Boolean = False        ' To detect redundant calls
    Public Shared DataDirectory As String = ""
    Public Event BeforeLoadAll()
    Public Event AfterLoadAll()
    Public Event BeforeSaveAll()
    Public Event AfterSaveAll()
    Public Event LoadingTable(ByVal fileName As String)
    Public Event SavingTable(ByVal fileName As String)

    Public Event BeforeLoadTable(ByVal table As DataTable)
    Public Event AfterLoadTable(ByVal table As DataTable)
    Public Event BeforeSaveTable(ByVal table As DataTable)
    Public Event AfterSaveTable(ByVal table As DataTable)

    ' IDisposable
    Protected Overridable Sub Dispose(ByVal disposing As Boolean)
        If Not Me.disposedValue Then
            If disposing Then
                ' TODO: free managed resources when explicitly called
                ds.Dispose()
            End If

            ' TODO: free shared unmanaged resources
        End If
        Me.disposedValue = True
    End Sub

    Public Shared ReadOnly Property BaseDirectory() As String
        Get
            Return DataDirectory & "TableData\"
        End Get
    End Property

    Public ReadOnly Property DataSet() As DataSet
        Get
            Return ds
        End Get
    End Property

    Public ReadOnly Property Table(ByVal tableName As String) As DataTable
        Get
            If ds.Tables.Contains(tableName) Then
                Return ds.Tables(tableName)
            End If
            Return Nothing
        End Get
    End Property

    Public Sub SaveSchema()
        ds.WriteXmlSchema(ds.DataSetName & ".xsd")
    End Sub

    Public Sub LoadSchema(ByVal schemaFileName As String)
        If ds IsNot Nothing Then
            ds.Clear()
            ds.Tables.Clear()
            ds.Relations.Clear()
            ds.Dispose()
        End If
        ds = New DataSet
        ds.ReadXmlSchema(schemaFileName)
        For Each t As DataTable In ds.Tables
            Dim handler As DefaultTable
            Dim handlerName As String = GetStringProperty(t, TableProperty.TableHandlerName)
            If handlerName Is Nothing Then
                handler = New DefaultTable(t, Me)
            Else
                Dim obj As Object = Activator.CreateInstance(Type.GetType("BackEnd." & handlerName), t, Me)
                handler = DirectCast(obj, DefaultTable)
            End If
            If t.ExtendedProperties.Contains(TableProperty.TableHandler) Then t.ExtendedProperties.Remove(TableProperty.TableHandler)
            t.ExtendedProperties.Add(TableProperty.TableHandler, handler)
        Next
    End Sub

    Public Function GetHandler(ByVal tableName As String) As DefaultTable
        If ds.Tables.Contains(tableName) Then
            Return GetTableHandler(ds.Tables(tableName))
        End If
        Return Nothing
    End Function

    Protected Sub BeginInit()
        ds.BeginInit()
        ds.EnforceConstraints = False
    End Sub

    Protected Sub EndInit()
        ds.AcceptChanges()
        Try
            ds.EnforceConstraints = True
        Catch ex As ConstraintException
            ErrorHandler.ShowError("One or more of your files contain invalid data.  Please fix the data and restart the editor.", "Error Loading Files", ex)
            For Each t As DataTable In ds.Tables
                If t.HasErrors Then
                    Dim errStr As New Text.StringBuilder("Details:" & vbCrLf & vbCrLf)
                    Dim fileName As String = GetStringProperty(t, TableProperty.FileName)
                    If fileName IsNot Nothing Then
                        errStr.Append("File: " & fileName & vbCrLf)
                    Else
                        errStr.Append("Table: " & t.TableName & vbCrLf)
                    End If
                    For i As Integer = 0 To t.GetErrors.GetUpperBound(0)
                        errStr.Append(vbCrLf & t.GetErrors(i).RowError)
                    Next
                    ErrorHandler.ShowError(errStr.ToString, "Error Loading Files", MessageBoxIcon.Exclamation)
                End If
            Next
            ErrorHandler.TriggerFatalError()
        End Try
        ds.EndInit()
    End Sub

    Public Overridable Sub LoadAllData()
        RaiseEvent BeforeLoadAll()
        ds.Clear()
        BeginInit()
        For Each t As DataTable In ds.Tables
            LoadData(t)
        Next
        EndInit()
        RaiseEvent AfterLoadAll()
    End Sub

    Public Overridable Sub LoadData(ByVal tableName As String)
        Dim t As DataTable = ds.Tables(tableName)
        If Not t Is Nothing Then LoadData(t)
    End Sub

    Public Overridable Sub LoadData(ByVal table As DataTable)
        Application.DoEvents()
        RaiseEvent BeforeLoadTable(table)
        Dim fileName As String = GetStringProperty(table, TableProperty.FileName)
        If fileName IsNot Nothing Then
            RaiseEvent LoadingTable(fileName)
            GetTableHandler(table).LoadData()
        End If
        RaiseEvent AfterLoadTable(table)
    End Sub

    Public Overridable Sub SaveAllData()
        RaiseEvent BeforeSaveAll()
        For Each t As DataTable In ds.Tables
            SaveData(t)
        Next
        RaiseEvent AfterSaveAll()
    End Sub
    Public Overridable Sub SaveData(ByVal tableName As String)
        Dim t As DataTable = ds.Tables(tableName)
        If Not t Is Nothing Then
            SaveData(t)
        End If
    End Sub

    Public Overridable Sub SaveData(ByVal table As DataTable)
        Application.DoEvents()
        RaiseEvent BeforeSaveTable(table)
        Dim fileName As String = GetStringProperty(table, TableProperty.FileName)
        If fileName IsNot Nothing Then
            RaiseEvent SavingTable(fileName)
            GetTableHandler(table).SaveData()
        End If
        RaiseEvent AfterSaveTable(table)
    End Sub

    'these only work when the pk is an integer, which it should be 99% of the time in our xml files anyway
    'they also just work on single pk tables for now
    Public Function GetNextPrimaryKeyValue(ByVal tableName As String) As Integer
        Return GetNextPrimaryKeyValue(ds.Tables(tableName))
    End Function
    Public Function GetNextPrimaryKeyValue(ByVal table As DataTable) As Integer
        Return GetTableHandler(table).GetNextPrimaryKeyValue
    End Function

    Public Function NewRow(ByVal tableName As String) As DataRow
        Return NewRow(ds.Tables(tableName))
    End Function
    Public Function NewRow(ByVal table As DataTable) As DataRow
        Return GetTableHandler(table).NewRow
    End Function

    Public Sub DeleteRow(ByVal tableName As String, ByVal key As Integer)
        DeleteRow(ds.Tables(tableName), key)
    End Sub
    Public Sub DeleteRow(ByVal table As DataTable, ByVal key As Integer)
        GetTableHandler(table).DeleteRow(key)
    End Sub

    Public Function DuplicateRow(ByVal tableName As String, ByVal key As Integer) As DataRow
        Return DuplicateRow(ds.Tables(tableName), key)
    End Function
    Public Function DuplicateRow(ByVal table As DataTable, ByVal key As Integer) As DataRow
        Return GetTableHandler(table).DuplicateRow(key)
    End Function


#Region " IDisposable Support "
    ' This code added by Visual Basic to correctly implement the disposable pattern.
    Public Sub Dispose() Implements IDisposable.Dispose
        ' Do not change this code.  Put cleanup code in Dispose(ByVal disposing As Boolean) above.
        Dispose(True)
        GC.SuppressFinalize(Me)
    End Sub
#End Region

End Class
