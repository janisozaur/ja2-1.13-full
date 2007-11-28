Public Class GridForm
    Protected view As DataView
    Protected subTable As String
    Protected origFilter As String
    Protected customFilter As String
    Private sizeProperty As String
    Private locationProperty As String
    Private stateProperty As String

    Public Sub New(ByVal formText As String, ByVal vw As DataView, Optional ByVal subTable As String = Nothing)
        LoadingForm.Show()
        Application.DoEvents()
        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        Me.Text = formText
        sizeProperty = Me.Text & "_Size"
        locationProperty = Me.Text & "_Location"
        stateProperty = Me.Text & "_WindowState"

        view = vw
        origFilter = view.RowFilter
        Me.subTable = subTable
        InitializeGrid(Grid, view, subTable)
    End Sub

    Public Property Filter() As String
        Get
            Return customFilter
        End Get
        Set(ByVal value As String)
            customFilter = value
            If customFilter IsNot Nothing AndAlso customFilter.Length > 0 Then
                Try
                    If origFilter IsNot Nothing AndAlso origFilter.Length > 0 Then
                        view.RowFilter = origFilter & " AND " & customFilter
                    Else
                        view.RowFilter = customFilter
                    End If
                Catch ex As Exception
                    ErrorHandler.ShowError("Invalid expression.  Check the tooltips in the column headings for the fieldnames.  A standard SQL expression is expected.", "Filter Error", ex)
                    view.RowFilter = origFilter
                End Try
            Else
                view.RowFilter = origFilter
            End If
        End Set
    End Property

    Private Sub Grid_SelectionChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles Grid.SelectionChanged
        UpdateStatusBar()
    End Sub

    Private Sub Grid_UserAddedRow(ByVal sender As Object, ByVal e As System.Windows.Forms.DataGridViewRowEventArgs) Handles Grid.UserAddedRow
        UpdateStatusBar()
    End Sub

    Private Sub Grid_UserDeletedRow(ByVal sender As Object, ByVal e As System.Windows.Forms.DataGridViewRowEventArgs) Handles Grid.UserDeletedRow
        UpdateStatusBar()
    End Sub

    Private Sub Grid_UserDeletingRow(ByVal sender As Object, ByVal e As System.Windows.Forms.DataGridViewRowCancelEventArgs) Handles Grid.UserDeletingRow
        DeleteGridRow(DirectCast(sender, DataGridView), e)
    End Sub

    Private Sub SelectColumnsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SelectColumnsToolStripMenuItem.Click
        Dim curState As FormWindowState = Me.WindowState
        Dim frm As New ColumnSelectForm(Me.Grid, subTable)
        frm.ShowDialog(Me)
        Me.Hide()
        Me.WindowState = FormWindowState.Normal
        LoadingForm.Show()
        InitializeGrid(Grid, view, subTable)
        LoadingForm.Close()
        Me.Show()
        Me.WindowState = curState
    End Sub

    Private Sub GridForm_Activated(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Activated
        UpdateStatusBar()
    End Sub

    Private Sub GridForm_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        SetPropertyValue(sizeProperty, GetType(Size), Me.Size)
        SetPropertyValue(locationProperty, GetType(Point), Me.Location)
        SetPropertyValue(stateProperty, GetType(FormWindowState), Me.WindowState)
    End Sub

    Private Sub SetPropertyValue(ByVal name As String, ByVal type As Type, ByVal value As Object)
        With My.Settings
            Dim propVal As Configuration.SettingsPropertyValue = .PropertyValues.Item(name)
            If propVal Is Nothing Then
                Dim prop As New Configuration.SettingsProperty(name, type, My.Settings.Providers(0), False, value, Configuration.SettingsSerializeAs.ProviderSpecific, Nothing, True, True)
                .Properties.Add(prop)
                propVal = New Configuration.SettingsPropertyValue(prop)
                .PropertyValues.Add(propVal)
            End If
            propVal.PropertyValue = value
        End With
    End Sub

    Private Function GetPropertyValue(ByVal name As String) As Object
        Dim propVal As Configuration.SettingsPropertyValue = My.Settings.PropertyValues.Item(name)
        If Not propVal Is Nothing Then Return propVal.PropertyValue
        Return Nothing
    End Function

    Private Sub GridForm_Load(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Load
        LoadingForm.Close()

        Dim savedSize As Size = GetPropertyValue(sizeProperty)
        If savedSize = New Size(0, 0) Then
            Me.Width = Math.Min(MainWindow.ClientRectangle.Width, GetActualGridWidth(Grid))
            Me.Height = Math.Min(MainWindow.ClientRectangle.Height, GetActualGridHeight(Grid))
        Else
            Me.Size = savedSize
        End If

        Dim savedLocation As Point = GetPropertyValue(locationProperty)
        Dim savedState As FormWindowState = GetPropertyValue(stateProperty)
        Me.Location = savedLocation
        Me.WindowState = savedState
    End Sub

    Private Sub FilterToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles FilterToolStripMenuItem.Click
        Dim frm As New CustomFilterForm
        frm.Filter = Me.Filter
        If frm.ShowDialog(Me) = Windows.Forms.DialogResult.OK Then Me.Filter = frm.Filter
    End Sub

    Protected Sub UpdateStatusBar()
        MainWindow.StatusLabel.Text = Me.Text & ": "
        If Grid.SelectedCells.Count > 0 Then
            MainWindow.StatusLabel.Text &= Grid.SelectedCells(0).RowIndex + 1 & " of "
        End If
        MainWindow.StatusLabel.Text &= view.Count & " Rows"
    End Sub

    Private Sub Form_VisibleChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.VisibleChanged
        If Not Me.Visible Then MainWindow.StatusLabel.Text = ""
    End Sub
End Class