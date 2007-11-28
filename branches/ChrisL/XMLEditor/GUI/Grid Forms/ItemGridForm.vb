Public Class ItemGridForm
    Inherits GridForm
    Protected deletingRow As DataRow
    Dim enableEditingMenuItem As ToolStripMenuItem

    Const EnableEditingText As String = "Enable &Editing"
    Const DuplicateText As String = "&Duplicate"
    Const NewText As String = "&New"
    Const ViewText As String = "&View Details"

    Public Sub New(ByVal formText As String, ByVal view As DataView, Optional ByVal subTable As String = Nothing)
        MyBase.New(formText, view, subTable)
        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        Me.Text = formText
        With Grid
            .AllowUserToDeleteRows = True
            .RowTemplate.Height = ItemImages.GreatestBigImageHeight
        End With

        enableEditingMenuItem = New ToolStripMenuItem(EnableEditingText, Nothing, AddressOf EnableEditingMenuItem_Click)
        enableEditingMenuItem.CheckState = CheckState.Unchecked

        GridContextMenu.Items.Add(ViewText, Nothing, AddressOf ViewMenuItem_Click)
        GridContextMenu.Items.Add(NewText, Nothing, AddressOf NewMenuItem_Click)
        GridContextMenu.Items.Add(DuplicateText, Nothing, AddressOf DuplicateMenuItem_Click)
        GridContextMenu.Items.Add(enableEditingMenuItem)
    End Sub

    Protected Sub Grid_CellMouseDoubleClick(ByVal sender As Object, ByVal e As System.Windows.Forms.DataGridViewCellMouseEventArgs) Handles Grid.CellMouseDoubleClick
        ItemDataForm.OpenItemDataForm(Grid.Rows(e.RowIndex).Cells(Tables.Items.Fields.ID).Value, Grid.Rows(e.RowIndex).Cells(Tables.Items.Fields.Name).Value)
    End Sub

    Private Sub Grid_UserDeletingRow(ByVal sender As Object, ByVal e As System.Windows.Forms.DataGridViewRowCancelEventArgs) Handles Grid.UserDeletingRow
        deletingRow = view.Table.Rows.Find(e.Row.Cells(Tables.Items.Fields.ID).Value)
    End Sub

    Private Sub Grid_UserDeletedRow(ByVal sender As Object, ByVal e As System.Windows.Forms.DataGridViewRowEventArgs) Handles Grid.UserDeletedRow
        If deletingRow IsNot Nothing AndAlso deletingRow.RowState = DataRowState.Deleted Then
            deletingRow.RejectChanges()
            DB.DeleteRow(view.Table, deletingRow(Tables.Items.Fields.ID))
        End If
    End Sub

    Private Sub ViewMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
        If Grid.SelectedRows.Count > 0 Then
            ItemDataForm.OpenItemDataForm(Grid.SelectedRows(0).Cells(Tables.Items.Fields.ID).Value, Grid.SelectedRows(0).Cells(Tables.Items.Fields.Name).Value)
        End If
    End Sub

    Private Sub NewMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
        Dim frm As New NewItemForm()
        If Not MainWindow.FormOpen(frm.Text) Then MainWindow.ShowForm(frm)
    End Sub

    Private Sub DuplicateMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
        If Grid.SelectedRows.Count > 0 Then
            Dim key As Integer = Grid.SelectedRows(0).Cells(Tables.Items.Fields.ID).Value
            Dim row As DataRow = DB.DuplicateRow(view.Table, key)

            ItemDataForm.OpenItemDataForm(row(Tables.Items.Fields.ID), row(Tables.Items.Fields.Name))
        End If
    End Sub

    Private Sub EnableEditingMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs)
        enableEditingMenuItem.Checked = Not enableEditingMenuItem.Checked
        With Grid
            .ReadOnly = Not enableEditingMenuItem.Checked
            If Not .ReadOnly Then
                .SelectionMode = DataGridViewSelectionMode.RowHeaderSelect
            Else
                .SelectionMode = DataGridViewSelectionMode.FullRowSelect
            End If
        End With
    End Sub

End Class