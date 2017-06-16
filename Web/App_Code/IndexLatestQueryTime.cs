using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

/// <summary>
/// IndexLatestQueryTime
/// </summary>
public class IndexLatestQueryTime
{
    public IndexLatestQueryTime()
    {
        //
        // 
        //
    }
    //SelfQuery LSH Token GetData FilterK TotalQuery
    //All is microsecond
    public long SelfQuery { get; set; }
    public long ComputeLSH { get; set; }
    public long ComputeToken { get; set; }
    public long GetData { get; set; }
    public long Filter { get; set; }
    public long Query { get; set; }
}