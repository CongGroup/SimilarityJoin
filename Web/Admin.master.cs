﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

public partial class Admin : System.Web.UI.MasterPage
{

    public PageInfo pageInfo;

    protected void Page_Load(object sender, EventArgs e)
    {

        if (!IsPostBack) {

            pageInfo = WebHelper.GetPageInfo(this);

        }
    }
}
