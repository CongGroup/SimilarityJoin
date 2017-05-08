using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

/// <summary>
/// Summary description for IndexMetaData
/// </summary>
public class IndexMetaData
{
    public IndexMetaData()
    {
        //
        // TODO: Add constructor logic here
        //
    }

    public int IndexCurrentNum { get; set; }
    public int IndexTotalNum { get; set; }
    public double IndexLoad { get; set; }
    public int IndexMemorySize { get; set; }
    public double MemoryUsePercent { get; set; }


}