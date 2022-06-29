using System;
using System.Windows.Forms;
using System.Data; //DataTable
using System.Reflection; //Assembly
using System.IO; //Directory
using Utility; //ILicense & IData

namespace LicenseScheme
{
    public partial class frmMain : Form
    {
        private Assembly da = null;

        public frmMain()
        {
            InitializeComponent();

            //setup grid
            DataTable dt = new DataTable();
            dt.Columns.Add("Key");
            dt.Columns.Add("Value");

            dgvOptions.DataSource = dt;

            try
            {
                da = Assembly.LoadFrom(Directory.GetCurrentDirectory() + @"\Utility_x86.dll"); //looking for x32 version                    
            }
            catch(Exception)
            {
                da = Assembly.LoadFrom(Directory.GetCurrentDirectory() + @"\Utility_x64.dll"); //looking for x64 version
            }
        }
        
        private void btnEncode_Click(object sender, EventArgs e)
        {
            //check for properly loaded assembly before we try to use
            if (da != null)
            {
                Type lt = da.GetType("Utility.License"); //get License type from loaded assembly
                ILicense ld = (ILicense)Activator.CreateInstance(lt); //instantiate interface

                ld.Customer = tbCustomer.Text;
                ld.Product = tbProduct.Text;
                ld.Version = tbVersion.Text;
                
                DataTable dt = (DataTable)dgvOptions.DataSource;
                ld.D = new IData[dt.Rows.Count];

                for (int i = 0; i < dt.Rows.Count; i++)
                {
                    Type dataType = da.GetType("Utility.Data"); //get Data type from loaded assembly
                    IData data = (IData)Activator.CreateInstance(dataType); //instantiate interface

                    data.Key = dt.Rows[i]["Key"].ToString();
                    data.Value = dt.Rows[i]["Value"].ToString();

                    ld.D[i] = data;
                }

                tbLicense.Text = ld.Save();
            }
        }

        private void btnDecode_Click(object sender, EventArgs e)
        {
            //check for properly loaded assembly before we try to use
            if (da != null)
            {
                Type lt = da.GetType("Utility.License"); //get License type from loaded assembly
                ILicense ld = (ILicense)Activator.CreateInstance(lt); //instantiate interface

                try
                {
                    ld.Open(tbLicense.Text);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Error");
                }

                tbCustomer.Text = ld.Customer;
                tbProduct.Text = ld.Product;
                tbVersion.Text = ld.Version;

                DataTable dt = new DataTable();
                dt.Columns.Add("Key");
                dt.Columns.Add("Value");

                foreach (IData l in ld.D)
                    dt.Rows.Add(l.Key, l.Value);

                dgvOptions.DataSource = dt;
            }
        }
    }
}
