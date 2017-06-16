using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

public partial class API_GetIndexJoinResult : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        ThriftAdapter adapter = new ThriftAdapter(WebConfig.ServerIP, WebConfig.ServerPort);
        adapter.Open();
        SimilarityJoinService.Client client = adapter.GetClient();

        string strJoinType = Request.Form["JoinType"];
        string strThresholdK = Request.Form["ThresholdK"];
        string strFilePath = Request.Form["FilePath"];
        string strSelfQueryR = Request.Form["SelfQueryR"];

        string filePath = strFilePath;
        int type = int.Parse(strJoinType);
        //default K is 88
        int thresholdK = int.Parse(strThresholdK);
        //default R is 0.2
        double selfQueryR = 0.2;
        if(strSelfQueryR != null && strSelfQueryR != "")
        {
            selfQueryR = double.Parse(strSelfQueryR);
        }
        int timeout = 600;
        List<string> joinList = new List<string>();

        StreamReader sr = new StreamReader(filePath, Encoding.Default);
        string line;
        while ((line = sr.ReadLine()) != null)
        {
            joinList.Add(line);
        }
        sr.Close();

        List<int> list;
        switch (type)
        {
            case 1:
                {
                    list = client.JoinByStrategy1(joinList, thresholdK, timeout);
                    break;
                }
            case 2:
                {
                    list = client.JoinByStrategy2(joinList, thresholdK, timeout);
                    break;
                }
            case 3:
                {
                    list = client.JoinByStrategy3(joinList, thresholdK, timeout, selfQueryR);
                    break;
                }
            default:
                {
                    list = new List<int>();
                    break;
                }
        }

        adapter.Close();

        IndexUserIDList joinResult = new IndexUserIDList();
        joinResult.count = list.Count;
        joinResult.Ids = list;

        string res = JsonHelper.Serialize(joinResult);
        Response.Write(res);
        Response.End();
    }
}