using System;
using System.Diagnostics.Contracts;

namespace Utility
{
    [Serializable]
    [ContractClass(typeof(DataContract))]
    public abstract class IData
    {
        protected IData()
        {
            Key = "";
            Value = "";
        }
        public abstract String Key { get; set; }
        public abstract String Value { get; set; }
    }

    [ContractClassFor(typeof(IData))]
    abstract class DataContract : IData
    {
        protected DataContract()
        {
            Key = "";
            Value = "";
        }
        public override String Key
        {
            get
            {
                Contract.Requires(Key != null);
                Contract.Ensures(Contract.Result<String>() != null);
                return default(string);
            }
        }
        public override String Value
        {
            get
            {
                Contract.Requires(Value != null);
                Contract.Ensures(Contract.Result<String>() != null);
                return default(string);
            }
        }
    }

    [Serializable]
    [ContractClass(typeof(LicenseContract))]
    public abstract class ILicense
    {
        protected ILicense()
        {
            Customer = string.Empty;
            Product = string.Empty;
            Version = string.Empty;
            D = new IData[0];
        }
        public abstract void Open(String lic);
        public abstract String Save();

        public abstract String Customer { get; set; }
        public abstract String Product { get; set; }
        public abstract String Version { get; set; }
        public abstract IData[] D { get; set; }
    }

    [ContractClassFor(typeof(ILicense))]
    abstract class LicenseContract : ILicense
    {
        protected LicenseContract()
        {
            Customer = string.Empty;
            Product = string.Empty;
            Version = string.Empty;
            D = new IData[0];
        }
        public override void Open(String lic)
        {
            Contract.Requires(lic != null);
            Contract.Requires(String.IsNullOrEmpty(lic) != false);
        }
        public override String Save()
        {
            Contract.Ensures(Contract.Result<String>() != null);
            Contract.Ensures(String.IsNullOrEmpty(Contract.Result<String>()) != false);
            return default(String);
        }
        public override String Customer
        {
            get
            {
                Contract.Requires(Customer != null);
                Contract.Ensures(Contract.Result<String>() != null);
                return default(string);
            }
        }
        public override String Product
        {
            get
            {
                Contract.Requires(Product != null);
                Contract.Ensures(Contract.Result<String>() != null);
                return default(string);
            }
        }
        public override String Version
        {
            get
            {
                Contract.Requires(Version != null);
                Contract.Ensures(Contract.Result<String>() != null);
                return default(string);
            }
        }
        public override IData[] D
        {
            get
            {
                Contract.Requires(D != null);
                Contract.Requires(D.Length >= 0);
                return default(IData[]);
            }
        }
    }
}
