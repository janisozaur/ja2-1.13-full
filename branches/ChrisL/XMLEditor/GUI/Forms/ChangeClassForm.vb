Public Class ChangeClassForm
    Inherits SimpleFormBase

    Protected view As DataView
    Protected form As ItemDataForm

    'the view argument here needs to be the single-row view from the itemdataform
    Public Sub New(ByVal vw As DataView, ByVal itemForm As ItemDataForm)

        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        Me.btnOK.Top = myOKButton.Top
        Me.btnCancel.Top = myCancelButton.Top

        Me.btnOK.Left = myOKButton.Left
        Me.btnCancel.Left = myCancelButton.Left

        view = vw
        form = itemForm

        With ItemClassListBox
            .DataSource = DB.Table(Tables.ItemClasses)
            .DisplayMember = Tables.LookupTableFields.Name
            .ValueMember = Tables.LookupTableFields.ID
            .SelectedValue = view(0)(Tables.Items.Fields.ItemClass)
        End With

    End Sub

    Protected Overrides Function OkAction() As Boolean
        Dim id As Integer = view(0)(Tables.Items.Fields.ID)
        view(0)(Tables.Items.Fields.ItemClass) = ItemClassListBox.SelectedValue
        view(0).EndEdit()

        view = Nothing

        Dim frm As New ItemDataForm(id, form.Text)
        frm.Top = form.Top
        frm.Left = form.Left

        form.Close()
        form = Nothing

        MainWindow.ShowForm(frm)
        Return True
    End Function
End Class