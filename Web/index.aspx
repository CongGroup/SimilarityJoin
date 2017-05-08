<%@ Page Title="" Language="C#" MasterPageFile="~/Admin.master" AutoEventWireup="true" CodeFile="index.aspx.cs" Inherits="index" %>

<%@ MasterType VirtualPath="Admin.master" %>

<asp:Content ID="Content1" ContentPlaceHolderID="botHead" runat="Server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="midBody" runat="Server">

    <div class="row">
        <div class="col-lg-2">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <span class="label label-info pull-right">Number</span>
                    <h5>Current Size</h5>
                </div>
                <div class="ibox-content">
                    <h1 class="no-margins">386,200</h1>
                    <div class="stat-percent font-bold text-info">Available <i class="fa fa-level-up"></i></div>
                    <small>Elements</small>
                </div>
            </div>
        </div>
        <div class="col-lg-2">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <span class="label label-info pull-right">Number</span>
                    <h5>Total Size</h5>
                </div>
                <div class="ibox-content">
                    <h1 class="no-margins">80,800</h1>
                    <div class="stat-percent font-bold text-info">Fixed <i class="fa fa-bolt"></i></div>
                    <small>Elements</small>
                </div>
            </div>
        </div>

        <div class="col-lg-4">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <span class="label label-primary pull-right">Number | Percent</span>
                    <h5>Index Load</h5>
                </div>
                <div class="ibox-content">

                    <div class="row">
                        <div class="col-md-6">
                            <h1 class="no-margins">406</h1>
                            <div class="font-bold text-navy">Room is available</div>
                        </div>
                        <div class="col-md-6">
                            <h1 class="no-margins">80%</h1>
                            <div class="font-bold text-navy">Capability Percent</div>
                        </div>
                    </div>


                </div>
            </div>
        </div>
        <div class="col-lg-4">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Memory Cost</h5>
                    <div class="ibox-tools">
                        <span class="label label-primary pull-right">Size | Percent</span>
                    </div>
                </div>
                <div class="ibox-content">

                    <div class="row">
                        <div class="col-md-6">
                            <h1 class="no-margins">406</h1>
                            <div class="font-bold text-navy">MB</div>
                        </div>
                        <div class="col-md-6">
                            <h1 class="no-margins">80%</h1>
                            <div class="font-bold text-navy">Memory Use Percent</div>
                        </div>
                    </div>

                </div>

            </div>
        </div>
    </div>


    <div class="row">
        <div class="col-lg-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Pie Chart Example</h5>
                    <div class="ibox-tools">
                        <a class="collapse-link">
                            <i class="fa fa-chevron-up"></i>
                        </a>
                        <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                            <i class="fa fa-wrench"></i>
                        </a>
                        <ul class="dropdown-menu dropdown-user">
                            <li><a href="#">Config option 1</a>
                            </li>
                            <li><a href="#">Config option 2</a>
                            </li>
                        </ul>
                        <a class="close-link">
                            <i class="fa fa-times"></i>
                        </a>
                    </div>
                </div>
                <div class="ibox-content">
                    <div class="flot-chart">
                        <div class="flot-chart-pie-content" id="flot-pie-chart"></div>
                    </div>
                </div>
            </div>
        </div>
        <div class="col-lg-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Live Chart Example</h5>
                    <div class="ibox-tools">
                        <a class="collapse-link">
                            <i class="fa fa-chevron-up"></i>
                        </a>
                        <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                            <i class="fa fa-wrench"></i>
                        </a>
                        <ul class="dropdown-menu dropdown-user">
                            <li><a href="#">Config option 1</a>
                            </li>
                            <li><a href="#">Config option 2</a>
                            </li>
                        </ul>
                        <a class="close-link">
                            <i class="fa fa-times"></i>
                        </a>
                    </div>
                </div>
                <div class="ibox-content">

                    <div class="flot-chart">
                        <div class="flot-chart-content" id="flot-line-chart-moving"></div>
                    </div>
                </div>
            </div>
        </div>
    </div>



</asp:Content>
<asp:Content ID="Content3" ContentPlaceHolderID="botBody" runat="Server">
    <script src="js/plugins/flot/jquery.flot.js"></script>
    <script src="js/plugins/flot/jquery.flot.tooltip.min.js"></script>
    <script src="js/plugins/flot/jquery.flot.pie.js"></script>
    <script>

        //Flot Pie Chart
        $(function () {

            var data = [{
                label: "Sales 1",
                data: 21,
                color: "#d3d3d3",
            }, {
                label: "Sales 2",
                data: 3,
                color: "#bababa",
            }, {
                label: "Sales 3",
                data: 15,
                color: "#79d2c0",
            }, {
                label: "Sales 4",
                data: 52,
                color: "#1ab394",
            }];

            var plotObj = $.plot($("#flot-pie-chart"), data, {
                series: {
                    pie: {
                        show: true
                    }
                },
                grid: {
                    hoverable: true
                },
                tooltip: true,
                tooltipOpts: {
                    content: "%p.0%, %s", // show percentages, rounding to 2 decimal places
                    shifts: {
                        x: 20,
                        y: 0
                    },
                    defaultTheme: false
                }
            });

        });


    </script>
</asp:Content>

