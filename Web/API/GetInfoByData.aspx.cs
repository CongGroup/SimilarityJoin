using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

public partial class API_GetInfoByData : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        string strFilePath = Request.Form["FilePath"];

        List<string> userDatas = new List<string>();

        StreamReader sr = new StreamReader(strFilePath, Encoding.Default);
        string line;
        while ((line = sr.ReadLine()) != null)
        {
            userDatas.Add(line);
        }
        sr.Close();

        //1 means query data type
        //2 means query data mate data
        //3 means query data type distribute
        int queryType = 1;

        ThriftAdapter adapter = new ThriftAdapter(WebConfig.ServerIP, WebConfig.ServerPort);
        adapter.Open();
        SimilarityJoinService.Client client = adapter.GetClient();
        List<string> typeList = client.QueryTypeByData(userDatas);
        adapter.Close();


        IndexUserTypeList indexUserTypeList = new IndexUserTypeList();
        indexUserTypeList.count = typeList.Count;
        indexUserTypeList.UserTypes = typeList;

        string res = JsonHelper.Serialize(indexUserTypeList);
        Response.Write(res);
        Response.End();
    }
}