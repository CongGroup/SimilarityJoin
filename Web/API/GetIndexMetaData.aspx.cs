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
        IndexMetaData indexMetaData = new IndexMetaData();

        indexMetaData.IndexCurrentNum = Int32.Parse(list[0]);
        indexMetaData.IndexTotalNum = Int32.Parse(list[1]);

    }
}