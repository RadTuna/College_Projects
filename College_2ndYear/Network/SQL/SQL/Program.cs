using System;
using System.Data;
using System.Data.SqlClient;

namespace SQL
{
    class Program
    {
        static void Main(string[] args)
        {
            SqlConnection myConnection = 
                new SqlConnection("Server=localhost;Integrated security=SSPI;database=GameDB");

            string str = "SELECT * from UserID";

                SqlCommand myCommand = new SqlCommand(str, myConnection);

            myConnection.Open();
            myCommand.ExecuteNonQuery();
            myConnection.Close();
        }
    }
}
