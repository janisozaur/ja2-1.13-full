Public Class ColumnSelectForm
    Inherits SimpleFormBase

    Dim table As DataTable
    Dim grid As DataGridView
    Dim subTable As String

    Public Sub New(ByVal grd As DataGridView, Optional ByVal subTable As String = Nothing)
        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        Me.btnOK.Top = myOKButton.Top
        Me.btnCancel.Top = myCancelButton.Top

        Me.btnOK.Left = myOKButton.Left
        Me.btnCancel.Left = myCancelButton.Left

        grid = grd
        Me.subTable = subTable
        table = DirectCast(grd.DataSource, DataView).Table

        GetColumnProperties()
    End Sub

    Protected Overrides Function OkAction() As Boolean
        SetColumnProperties()
        'rewrite the schema so it'll remember for next time the app starts
        DB.SaveSchema()
        Return True
    End Function

    Private Sub GetColumnProperties()
        For Each c As DataColumn In table.Columns
            If Not GetBooleanProperty(c, ColumnProperty.SubTable) OrElse (subTable IsNot Nothing AndAlso subTable.Length > 0 AndAlso c.ColumnName.StartsWith(subTable)) Then
                ColumnCheckList.Items.Add(c.Caption, Not GetBooleanProperty(c, ColumnProperty.Grid_Hidden))
            End If
        Next
    End Sub

    Private Sub SetColumnProperties()
        For Each c As DataColumn In table.Columns
            If Not GetBooleanProperty(c, ColumnProperty.SubTable) OrElse (subTable IsNot Nothing AndAlso subTable.Length > 0 AndAlso c.ColumnName.StartsWith(subTable)) Then
                If ColumnCheckList.CheckedItems.Contains(c.Caption) Then
                    RemoveProperty(table.Columns(c.ColumnName), ColumnProperty.Grid_Hidden)

                    If GetBooleanProperty(c, ColumnProperty.SubTable) Then
                        RemoveProperty(DB.Table(subTable).Columns(c.ColumnName.Remove(0, subTable.Length)), ColumnProperty.Grid_Hidden)
                    End If

                    If grid.Columns.Contains(c.ColumnName & LookupColumnSuffix) Then
                        grid.Columns(c.ColumnName & LookupColumnSuffix).Visible = True
                    Else
                        grid.Columns(c.ColumnName).Visible = True
                    End If
                Else
                    SetProperty(c, ColumnProperty.Grid_Hidden, True)
                    If GetBooleanProperty(c, ColumnProperty.SubTable) Then
                        SetProperty(DB.Table(subTable).Columns(c.ColumnName.Remove(0, subTable.Length)), ColumnProperty.Grid_Hidden, True)
                    End If

                    If grid.Columns.Contains(c.ColumnName) Then
                        grid.Columns(c.ColumnName).Visible = False
                        End If
                    If grid.Columns.Contains(c.ColumnName & LookupColumnSuffix) Then
                        grid.Columns(c.ColumnName & LookupColumnSuffix).Visible = False
                    End If
                End If
            End If
        Next

    End Sub

End Class