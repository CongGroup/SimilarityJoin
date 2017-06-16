using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

/// <summary>
/// IndexUserDataList
/// </summary>
public class IndexUserDataList
{
    public IndexUserDataList()
    {

    }
    public int count;
    /// <summary>
    /// each line is one user income, use space for 
    /// </summary>
    public List<string> UserData { get; set; }
}