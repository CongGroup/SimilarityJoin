using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;


public partial class API_GetIndexMetaData : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        ThriftAdapter adapter = new ThriftAdapter(WebConfig.ServerIP, WebConfig.ServerPort);
        adapter.Open();
        SimilarityJoinService.Client client = adapter.GetClient();
        List<string> list = client.GetIndexMetaData();
        adapter.Close();


        IndexMetaData indexMetaData = new IndexMetaData();

        indexMetaData.IndexCurrentNum = Int32.Parse(list[0]);
        indexMetaData.IndexTotalNum = Int32.Parse(list[1]);
        indexMetaData.IndexLoad = ((indexMetaData.IndexTotalNum - indexMetaData.IndexCurrentNum) * 10000 / indexMetaData.IndexTotalNum) / 100D;
        indexMetaData.IndexMemorySize = Int32.Parse(list[2]);
        int totalMemory = Int32.Parse(list[3]);
        indexMetaData.MemoryUsePercent = (indexMetaData.IndexMemorySize * 10000 / totalMemory) / 100D;

        string res = JsonHelper.Serialize(indexMetaData);
        Response.Write(res);
        Response.End();

    }
}