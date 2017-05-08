using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using Thrift.Protocol;
using Thrift.Collections;
using Thrift.Server;
using Thrift.Transport;


/// <summary>
/// Summary description for ThriftAdapter
/// </summary>
public class ThriftAdapter
{
    public ThriftAdapter(string ip = "127.0.0.1", int port = 9090)
    {
        transport = new TSocket(ip, port);
        protocol = new TBinaryProtocol(transport);
        client = new SimilarityJoinService.Client(protocol);
    }

    private TTransport transport;
    private TProtocol protocol;
    private SimilarityJoinService.Client client;

    private static Dictionary<string, ThriftAdapter> thriftDict = new Dictionary<string, ThriftAdapter>();

    public static ThriftAdapter Add(string skey, ThriftAdapter adapter)
    {
        thriftDict.Add(skey, adapter);
        return adapter;
    }

    public static ThriftAdapter Del(string sKey)
    {
        ThriftAdapter adapter = thriftDict[sKey];
        thriftDict.Remove(sKey);
        return adapter;
    }

    public static ThriftAdapter Get(string skey)
    {
        if (thriftDict.ContainsKey(skey))
        {
            return thriftDict[skey];
        }
        else
        {
            ThriftAdapter adapter = new ThriftAdapter();
            thriftDict[skey] = adapter;
            return adapter;
        }
    }


    public ThriftAdapter Open()
    {
        if (!transport.IsOpen)
        {
            transport.Open();
        }
        return this;
    }

    public ThriftAdapter Close()
    {
        if (transport.IsOpen)
        {
            transport.Close();
        }
        return this;
    }

    public SimilarityJoinService.Client GetClient()
    {
        return client;
    }
}