using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

public partial class API_GetInfoByID : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        string strIDList = Request.Form["JsonIdList"];
        string strQueryType = Request.Form["Type"];

        IndexUserIDList jsonIdList = JsonHelper.Deserialize<IndexUserIDList>(strIDList);
        List<int> userIds = jsonIdList.Ids;

        //1 means query data type
        //2 means query data mate data
        //3 means query data type distribute
        int queryType = int.Parse(strQueryType);

        string res = "";

        switch (queryType)
        {
            case 1:
                {
                    ThriftAdapter adapter = new ThriftAdapter(WebConfig.ServerIP, WebConfig.ServerPort);
                    adapter.Open();
                    SimilarityJoinService.Client client = adapter.GetClient();
                    List<string> typeList = client.QueryDataByID(userIds);
                    adapter.Close();

                    IndexUserTypeList indexUserTypeList = new IndexUserTypeList();
                    indexUserTypeList.count = typeList.Count;
                    indexUserTypeList.UserTypes = typeList;

                    res = JsonHelper.Serialize(indexUserTypeList);
                    break;
                }
            case 2:
                {
                    ThriftAdapter adapter = new ThriftAdapter(WebConfig.ServerIP, WebConfig.ServerPort);
                    adapter.Open();
                    SimilarityJoinService.Client client = adapter.GetClient();
                    List<string> dataList = client.QueryTypeByID(userIds);
                    adapter.Close();

                    IndexUserDataList indexUserDataList = new IndexUserDataList();
                    indexUserDataList.count = dataList.Count;
                    indexUserDataList.UserData = dataList;

                    res = JsonHelper.Serialize(indexUserDataList);
                    break;
                }
            case 3:
                {
                    ThriftAdapter adapter = new ThriftAdapter(WebConfig.ServerIP, WebConfig.ServerPort);
                    adapter.Open();
                    SimilarityJoinService.Client client = adapter.GetClient();
                    List<int> list = client.QueryDistributedByID(userIds);
                    adapter.Close();

                    IndexDistribution userDistribution = new IndexDistribution();
                    userDistribution.Labels = new string[WebConfig.FinancialKind];
                    userDistribution.Numbers = new int[WebConfig.FinancialKind];

                    for (int i = 0; i < WebConfig.FinancialKind; i++)
                    {
                        userDistribution.Labels[i] = "VIP" + (i + 1).ToString();
                        userDistribution.Numbers[i] = list[i];
                    }

                    res = JsonHelper.Serialize(userDistribution);
                    break;
                }
            default:
                {
                    res = JsonHelper.Serialize("Error Query Type");
                    break;
                }
        }
        Response.Write(res);
        Response.End();
    }
}