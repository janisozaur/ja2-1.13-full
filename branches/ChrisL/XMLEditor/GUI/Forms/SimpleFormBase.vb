Public Class SimpleFormBase

    Public Sub New()

        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        Me.AcceptButton = btnOK
    End Sub

    Private Sub CancelButton_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles btnCancel.Click
        Close()
    End Sub

    Private Sub OKButton_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles btnOK.Click
        If OkAction() Then Close()
    End Sub

    Protected Overridable Function OkAction() As Boolean
        Return True
    End Function

    Private Sub SimpleFormBase_Load(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Load
        Me.WindowState = FormWindowState.Normal
    End Sub
End Class