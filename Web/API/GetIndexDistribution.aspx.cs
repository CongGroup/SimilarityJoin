using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

public partial class API_GetIndexDistribution : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        ThriftAdapter adapter = new ThriftAdapter(WebConfig.ServerIP, WebConfig.ServerPort);
        adapter.Open();
        SimilarityJoinService.Client client = adapter.GetClient();
        List<int> list = client.GetIndexDistributed();
        adapter.Close();


        IndexDistribution indexDistribution = new IndexDistribution();
        indexDistribution.Labels = new string[WebConfig.FinancialKind];
        indexDistribution.Numbers = new int[WebConfig.FinancialKind];

        for (int i = 0; i < WebConfig.FinancialKind; i++)
        {
            indexDistribution.Labels[i] = "VIP" + (i + 1).ToString();
            indexDistribution.Numbers[i] = list[i];
        }

        string res = JsonHelper.Serialize(indexDistribution);
        Response.Write(res);
        Response.End();
    }
}