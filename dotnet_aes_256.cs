//Try it on https://dotnetfiddle.net/
using System;
using System.Security.Cryptography;
using System.Text;
using System.IO;


					
public class Program
{
	public static void Main()
	{
		string msg;
		byte[] encArray;
		msg = "Whatsupmyboaii";
		Console.WriteLine("Initial Message: " + msg);
		string password;
		byte[] array = GenerateRandomSalt();
		password = "Password1";
		byte[] bytes = Encoding.UTF8.GetBytes(password);
		RijndaelManaged rijndaelManaged = new RijndaelManaged();
		rijndaelManaged.KeySize = 256;
		rijndaelManaged.BlockSize = 128;
		Rfc2898DeriveBytes rfc2898DeriveBytes = new Rfc2898DeriveBytes(bytes, array, 50000);
		rijndaelManaged.Key = rfc2898DeriveBytes.GetBytes(rijndaelManaged.KeySize / 8);
		rijndaelManaged.IV = rfc2898DeriveBytes.GetBytes(rijndaelManaged.BlockSize / 8);
		rijndaelManaged.Padding = PaddingMode.PKCS7;
		rijndaelManaged.Mode = CipherMode.CBC;
		MemoryStream msEncrypt = new MemoryStream();
		CryptoStream cryptoStream = new CryptoStream(msEncrypt, rijndaelManaged.CreateEncryptor(), CryptoStreamMode.Write);
		using (StreamWriter Encrypt = new StreamWriter(cryptoStream))
        {
            Encrypt.Write(msg);
        }
		encArray = msEncrypt.ToArray();
		
		
		string text = null;
		Console.WriteLine("Encrypted data");
		Console.WriteLine(Encoding.UTF8.GetString(encArray));
		RijndaelManaged rijndaelManaged2 = new RijndaelManaged();
		Rfc2898DeriveBytes rfc2898DeriveBytes1 = new Rfc2898DeriveBytes(bytes, encArray, 50000);
		rijndaelManaged2.Key = rfc2898DeriveBytes1.GetBytes(rijndaelManaged.KeySize / 8);
		rijndaelManaged2.IV = rfc2898DeriveBytes1.GetBytes(rijndaelManaged.BlockSize / 8);
		rijndaelManaged2.Padding = PaddingMode.PKCS7;
		rijndaelManaged2.Mode = CipherMode.CBC;
		MemoryStream msEncrypt2 = new MemoryStream(encArray);
		CryptoStream cryptoStream2 = new CryptoStream(msEncrypt2, rijndaelManaged.CreateDecryptor(), CryptoStreamMode.Read);
		using (StreamReader Decrypt = new StreamReader(cryptoStream2))
        {
            text = Decrypt.ReadToEnd();
        }
		Console.WriteLine("Decrypted Data");
        Console.WriteLine(text);
		
		
	}
	
	public static byte[] GenerateRandomSalt()
		{
			byte[] array = new byte[32];
			using (RNGCryptoServiceProvider rngcryptoServiceProvider = new RNGCryptoServiceProvider())
			{
				for (int i = 0; i < 10; i++)
				{
					rngcryptoServiceProvider.GetBytes(array);
				}
			}
			return array;
		}
	
}
