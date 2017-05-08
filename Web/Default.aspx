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
            <h3>Welcome to Financial Demo</h3>
            <p>
                By the technique of similarity join, we can find a group of people which has similarity features with the specified group.
            </p>
            <p>Login in. To see it in action.</p>
            <form class="m-t" role="form" action="index.aspx">
                <div class="form-group">
                    <input type="text" name="user" class="form-control" placeholder="User" required="" />
                </div>
                <div class="form-group">
                    <input type="password" class="form-control" placeholder="Password" required="" />
                </div>
                <button type="submit" class="btn btn-primary block full-width m-b">Login</button>

                <a href="#"><small>Forget password ?</small></a>
            </form>
            <p class="m-t"> <small>Similarity Join Demo | ITF Project 版权所有 &copy; 2017</small> </p>
        </div>
    </div>

    <!-- Mainly scripts -->
    <script src="js/jquery-3.1.1.min.js"></script>
    <script src="js/bootstrap.min.js"></script>

</body>

</html>
