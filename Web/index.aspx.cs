using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

public partial class index : System.Web.UI.Page
{

    public PageInfo pageInfo;

    protected void Page_Load(object sender, EventArgs e)
    {
        if (!IsPostBack)
        {
            //Setting PageInfo
            pageInfo = WebHelper.GetPageInfo(this);
            pageInfo.PageTag = WebConfig.IndexPageTag;

            //Read User
            if(Request.QueryString[WebConfig.LoginUserID] != null)
            {
                pageInfo.User = Request.QueryString[WebConfig.LoginUserID] as string;
            }

        }
    }
}