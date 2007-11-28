Public Class DataGridForm
    Inherits GridForm

    Public Sub New(ByVal formText As String, ByVal view As DataView)
        MyBase.New(formText, view)
        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        Me.Text = formText
        With Me.Grid
            .AllowUserToDeleteRows = True
            .AllowUserToAddRows = True
            .SelectionMode = DataGridViewSelectionMode.RowHeaderSelect
            .RowHeadersVisible = True
            .ReadOnly = False
        End With
    End Sub

    Private Sub Grid_DefaultValuesNeeded(ByVal sender As Object, ByVal e As System.Windows.Forms.DataGridViewRowEventArgs) Handles Grid.DefaultValuesNeeded
        If view.Table.PrimaryKey.Length = 1 Then 'only works w/single keyed tables
            Dim key As String = view.Table.PrimaryKey(0).ColumnName
            Dim val As Integer = DB.GetNextPrimaryKeyValue(view.Table)
            e.Row.Cells(key).Value = val
        End If
    End Sub
End Class