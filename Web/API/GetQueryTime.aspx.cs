using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

public partial class API_GetQueryTime : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        ThriftAdapter adapter = new ThriftAdapter(WebConfig.ServerIP, WebConfig.ServerPort);
        adapter.Open();
        SimilarityJoinService.Client client = adapter.GetClient();
        List<string> list = client.GetLatestQueryTime();
        adapter.Close();


        IndexLatestQueryTime indexQueryTime = new IndexLatestQueryTime();

        indexQueryTime.SelfQuery = Int64.Parse(list[0]);
        indexQueryTime.ComputeLSH = Int64.Parse(list[1]);
        indexQueryTime.ComputeToken = Int64.Parse(list[2]);
        indexQueryTime.GetData = Int64.Parse(list[3]);
        indexQueryTime.Filter = Int64.Parse(list[4]);
        indexQueryTime.Query = Int64.Parse(list[5]);


        string res = JsonHelper.Serialize(indexQueryTime);
        Response.Write(res);
        Response.End();
    }
}