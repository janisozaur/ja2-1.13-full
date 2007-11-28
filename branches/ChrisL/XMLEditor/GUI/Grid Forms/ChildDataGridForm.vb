Public Class ChildDataGridForm
    Inherits LookupGridForm

    Public Sub New(ByVal formText As String, ByVal view As DataView)
        MyBase.New(formText, view)

        ' This call is required by the Windows Form Designer.
        InitializeComponent()

        ' Add any initialization after the InitializeComponent() call.
        Me.Text = formText
        With Me.Grid
            .AllowUserToAddRows = True
            .AllowUserToDeleteRows = True
        End With
    End Sub

End Class