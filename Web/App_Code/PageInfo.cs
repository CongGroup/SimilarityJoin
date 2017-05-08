using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

/// <summary>
/// Summary description for PageInfo
/// </summary>
public class PageInfo
{

    public PageInfo()
    {
        PageTag = "Index";
        Position = "Test Role";
        User = "root";
        IndexRealNum = "Loading";
        IndexAllNum = "Loading";
        IndexAllSize = "Loading";
    }

    /// <summary>
    /// PageTag : Index or Query
    /// </summary>
    public string PageTag { get; set; }

    public string User { get; set; }

    public string Position { get; set; }


    public string IndexRealNum { get; set; }
    public string IndexAllNum { get; set; }
    public string IndexAllSize { get; set; }

}