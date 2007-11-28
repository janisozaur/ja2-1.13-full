Public Class LoadingForm

    Public Shadows Sub Show(Optional ByVal saving As Boolean = False)
        If saving Then InfoLabel.Text = "Saving..."
        MyBase.Show()
    End Sub
End Class