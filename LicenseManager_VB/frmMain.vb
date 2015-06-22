Imports System.Data 'DataTable
Imports System.Reflection 'Assembly
Imports System.IO 'Directory
Imports System.Collections.Generic 'Dictionary
Imports Utility 'ILicense & IData

Public Class frmMain

    Private da As Assembly = Nothing

    Private Sub frmMain_Load(sender As System.Object, e As System.EventArgs) Handles MyBase.Load
        Dim dt As New DataTable()

        dt.Columns.Add("Key")
        dt.Columns.Add("Value")

        dgvOptions.DataSource = dt

        Try
            Dim assembly As Assembly = assembly.GetExecutingAssembly()
            Dim manifestModule As [Module] = assembly.ManifestModule
            Dim peKind As PortableExecutableKinds
            Dim machine As ImageFileMachine
            manifestModule.GetPEKind(peKind, machine)

            'load x32 or x64 managed wrapper which depends on directory
            If machine.ToString.Contains("86") Then
                da = assembly.LoadFrom(Directory.GetCurrentDirectory() + "\Utility_x86.dll") 'looking for x86 version
            Else
                da = assembly.LoadFrom(Directory.GetCurrentDirectory() + "\Utility_x64.dll") 'looking for x64 version
            End If
        Catch ex As Exception
            System.Diagnostics.Trace.Write(ex.Message)
            btnEncode.Enabled = False
            btnDecode.Enabled = False
        End Try
    End Sub

    Private Sub btnEncode_Click(sender As System.Object, e As System.EventArgs) Handles btnEncode.Click
        If da IsNot Nothing Then
            Dim lt As Type = da.GetType("Utility.License") 'get License type from loaded assembly
            Dim ld As ILicense = DirectCast(Activator.CreateInstance(lt), ILicense) 'instantiate interface

            ld.Customer = tbCustomer.Text
            ld.Product = tbProduct.Text
            ld.Version = tbVersion.Text

            Dim dt As DataTable = DirectCast(dgvOptions.DataSource, DataTable)
            'ld.D = New IData(dt.Rows.Count - 1) {}
            ld.Dict = New Dictionary(Of String, String)()

            For i As Integer = 0 To dt.Rows.Count - 1
                'Dim dataType As Type = da.GetType("Utility.Data") 'get License type from loaded assembly
                'Dim data As IData = DirectCast(Activator.CreateInstance(dataType), IData) 'instantiate interface

                'data.Key = dt.Rows(i)("Key").ToString
                'data.Value = dt.Rows(i)("Value").ToString

                'ld.D(i) = data
                Try
                    ld.Dict.Add(dt.Rows(i)("Key").ToString, dt.Rows(i)("Value").ToString)
                Catch ex As Exception
                    MessageBox.Show("Key Column MUST be unique", "Error", MessageBoxButtons.OK)
                End Try
            Next

            tbLicense.Text = ld.Save()
        End If
    End Sub

    Private Sub btnDecode_Click(sender As System.Object, e As System.EventArgs) Handles btnDecode.Click
        If da IsNot Nothing Then
            Dim lt As Type = da.GetType("Utility.License") 'get License type from loaded assembly
            Dim ld As ILicense = DirectCast(Activator.CreateInstance(lt), ILicense) 'instantiate interface

            Try
                ld.Open(tbLicense.Text)
            Catch ex As Exception
                MessageBox.Show(ex.Message, "Error")
            End Try

            tbCustomer.Text = ld.Customer
            tbProduct.Text = ld.Product
            tbVersion.Text = ld.Version

            Dim dt As New DataTable
            dt.Columns.Add("Key")
            dt.Columns.Add("Value")

            'For Each l As IData In ld.D
            '    dt.Rows.Add(l.Key, l.Value)
            'Next

            For Each item As KeyValuePair(Of String, String) In ld.Dict
                dt.Rows.Add(item.Key, item.Value)
            Next

            dgvOptions.DataSource = dt
        End If
    End Sub

End Class
