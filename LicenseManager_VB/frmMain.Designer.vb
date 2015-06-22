<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmMain
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Dim DataGridViewCellStyle4 As System.Windows.Forms.DataGridViewCellStyle = New System.Windows.Forms.DataGridViewCellStyle()
        Me.tbVersion = New System.Windows.Forms.TextBox()
        Me.label3 = New System.Windows.Forms.Label()
        Me.tbProduct = New System.Windows.Forms.TextBox()
        Me.tbCustomer = New System.Windows.Forms.TextBox()
        Me.btnDecode = New System.Windows.Forms.Button()
        Me.btnEncode = New System.Windows.Forms.Button()
        Me.tbLicense = New System.Windows.Forms.TextBox()
        Me.dgvOptions = New System.Windows.Forms.DataGridView()
        Me.label1 = New System.Windows.Forms.Label()
        Me.label2 = New System.Windows.Forms.Label()
        CType(Me.dgvOptions, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'tbVersion
        '
        Me.tbVersion.Location = New System.Drawing.Point(309, 27)
        Me.tbVersion.Name = "tbVersion"
        Me.tbVersion.Size = New System.Drawing.Size(104, 20)
        Me.tbVersion.TabIndex = 27
        '
        'label3
        '
        Me.label3.AutoSize = True
        Me.label3.Location = New System.Drawing.Point(306, 10)
        Me.label3.Name = "label3"
        Me.label3.Size = New System.Drawing.Size(42, 13)
        Me.label3.TabIndex = 26
        Me.label3.Text = "Version"
        '
        'tbProduct
        '
        Me.tbProduct.Location = New System.Drawing.Point(154, 27)
        Me.tbProduct.Name = "tbProduct"
        Me.tbProduct.Size = New System.Drawing.Size(149, 20)
        Me.tbProduct.TabIndex = 25
        '
        'tbCustomer
        '
        Me.tbCustomer.Location = New System.Drawing.Point(7, 27)
        Me.tbCustomer.Name = "tbCustomer"
        Me.tbCustomer.Size = New System.Drawing.Size(141, 20)
        Me.tbCustomer.TabIndex = 24
        '
        'btnDecode
        '
        Me.btnDecode.Location = New System.Drawing.Point(213, 272)
        Me.btnDecode.Name = "btnDecode"
        Me.btnDecode.Size = New System.Drawing.Size(75, 23)
        Me.btnDecode.TabIndex = 23
        Me.btnDecode.Text = "Decode"
        Me.btnDecode.UseVisualStyleBackColor = True
        '
        'btnEncode
        '
        Me.btnEncode.Location = New System.Drawing.Point(132, 272)
        Me.btnEncode.Name = "btnEncode"
        Me.btnEncode.Size = New System.Drawing.Size(75, 23)
        Me.btnEncode.TabIndex = 22
        Me.btnEncode.Text = "Encode"
        Me.btnEncode.UseVisualStyleBackColor = True
        '
        'tbLicense
        '
        Me.tbLicense.Location = New System.Drawing.Point(7, 301)
        Me.tbLicense.Multiline = True
        Me.tbLicense.Name = "tbLicense"
        Me.tbLicense.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.tbLicense.Size = New System.Drawing.Size(406, 214)
        Me.tbLicense.TabIndex = 21
        '
        'dgvOptions
        '
        DataGridViewCellStyle4.BackColor = System.Drawing.Color.FromArgb(CType(CType(224, Byte), Integer), CType(CType(224, Byte), Integer), CType(CType(224, Byte), Integer))
        Me.dgvOptions.AlternatingRowsDefaultCellStyle = DataGridViewCellStyle4
        Me.dgvOptions.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill
        Me.dgvOptions.ColumnHeadersHeight = 20
        Me.dgvOptions.Location = New System.Drawing.Point(7, 53)
        Me.dgvOptions.Name = "dgvOptions"
        Me.dgvOptions.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.dgvOptions.Size = New System.Drawing.Size(406, 213)
        Me.dgvOptions.TabIndex = 20
        '
        'label1
        '
        Me.label1.AutoSize = True
        Me.label1.Location = New System.Drawing.Point(7, 10)
        Me.label1.Name = "label1"
        Me.label1.Size = New System.Drawing.Size(51, 13)
        Me.label1.TabIndex = 18
        Me.label1.Text = "Customer"
        '
        'label2
        '
        Me.label2.AutoSize = True
        Me.label2.Location = New System.Drawing.Point(151, 10)
        Me.label2.Name = "label2"
        Me.label2.Size = New System.Drawing.Size(44, 13)
        Me.label2.TabIndex = 19
        Me.label2.Text = "Product"
        '
        'frmMain
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(420, 525)
        Me.Controls.Add(Me.tbVersion)
        Me.Controls.Add(Me.label3)
        Me.Controls.Add(Me.tbProduct)
        Me.Controls.Add(Me.tbCustomer)
        Me.Controls.Add(Me.btnDecode)
        Me.Controls.Add(Me.btnEncode)
        Me.Controls.Add(Me.tbLicense)
        Me.Controls.Add(Me.dgvOptions)
        Me.Controls.Add(Me.label1)
        Me.Controls.Add(Me.label2)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle
        Me.Name = "frmMain"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "License Creation"
        CType(Me.dgvOptions, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Private WithEvents tbVersion As System.Windows.Forms.TextBox
    Private WithEvents label3 As System.Windows.Forms.Label
    Private WithEvents tbProduct As System.Windows.Forms.TextBox
    Private WithEvents tbCustomer As System.Windows.Forms.TextBox
    Private WithEvents btnDecode As System.Windows.Forms.Button
    Private WithEvents btnEncode As System.Windows.Forms.Button
    Private WithEvents tbLicense As System.Windows.Forms.TextBox
    Private WithEvents dgvOptions As System.Windows.Forms.DataGridView
    Private WithEvents label1 As System.Windows.Forms.Label
    Private WithEvents label2 As System.Windows.Forms.Label

End Class
