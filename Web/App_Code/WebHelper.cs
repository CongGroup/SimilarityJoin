using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;

/// <summary>
/// Summary description for WebHelper
/// </summary>
public class WebHelper
{

    public static PageInfo GetPageInfo(MasterPage masterPage)
    {
        if(masterPage.Session[WebConfig.PageInfoSessionName] == null)
        {
            masterPage.Session[WebConfig.PageInfoSessionName] = new PageInfo();
        }

        return masterPage.Session[WebConfig.PageInfoSessionName] as PageInfo;
    }

    public static PageInfo GetPageInfo(Page page)
    {
        if (page.Session[WebConfig.PageInfoSessionName] == null)
        {
            page.Session[WebConfig.PageInfoSessionName] = new PageInfo();
        }

        return page.Session[WebConfig.PageInfoSessionName] as PageInfo;
    }

}