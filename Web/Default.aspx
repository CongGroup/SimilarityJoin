<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Default.aspx.cs" Inherits="_Default" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">

    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />

    <title>ITF Similarity Join Demo | Login</title>

    <link href="css/bootstrap.min.css" rel="stylesheet">
    <link href="font-awesome/css/font-awesome.css" rel="stylesheet">

    <link href="css/animate.css" rel="stylesheet">
    <link href="css/style.css" rel="stylesheet">
</head>

<body class="gray-bg">

    <div class="middle-box text-center loginscreen animated fadeInDown">
        <div>
            <div>

                <h1 class="logo-name">ITF</h1>

            </div>
            <h3>欢迎使用CyberX医院管理示例</h3>
            <p>
                我们的医院管理系统使用BlindDB加密数据库，所有的敏感数据都已进行加密处理。
            </p>
            <p>请输入用户名与密码，使用不同角色进入系统</p>
            <form class="m-t" role="form" action="index.aspx">
                <div class="form-group">
                    <input type="text" class="form-control" placeholder="用户名" required="" />
                </div>
                <div class="form-group">
                    <input type="password" class="form-control" placeholder="密码" required="" />
                </div>
                <button type="submit" class="btn btn-primary block full-width m-b">登 录</button>

                <a href="#"><small>忘记密码 ？</small></a>
            </form>
            <p class="m-t"> <small>医院管理系统 | CyberX Tech Limited 版权所有 &copy; 2017</small> </p>
        </div>
    </div>

    <!-- Mainly scripts -->
    <script src="js/jquery-3.1.1.min.js"></script>
    <script src="js/bootstrap.min.js"></script>

</body>

</html>
