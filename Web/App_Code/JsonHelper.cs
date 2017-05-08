using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Script.Serialization;

/// <summary>
/// Summary description for JsonHelper
/// </summary>
public class JsonHelper
{

    /// <summary>
    /// 把json字符串转成对象
    /// </summary>
    /// <typeparam name="T">对象</typeparam>
    /// <param name="data">json字符串</param> 
    public static T Deserialize<T>(string data)
    {
        System.Web.Script.Serialization.JavaScriptSerializer json = new System.Web.Script.Serialization.JavaScriptSerializer();
        return json.Deserialize<T>(data);
    }

    /// <summary>
    /// 把对象转成json字符串
    /// </summary>
    /// <param name="o">对象</param>
    /// <returns>json字符串</returns>
    public static string Serialize(object o)
    {
        System.Text.StringBuilder sb = new System.Text.StringBuilder();
        System.Web.Script.Serialization.JavaScriptSerializer json = new System.Web.Script.Serialization.JavaScriptSerializer();
        json.Serialize(o, sb);
        return sb.ToString();
    }

}
