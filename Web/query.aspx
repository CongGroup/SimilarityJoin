<%@ Page Title="" Language="C#" MasterPageFile="~/Admin.master" AutoEventWireup="true" CodeFile="query.aspx.cs" Inherits="query" %>

<asp:Content ID="Content1" ContentPlaceHolderID="botHead" runat="Server">

    <link href="css/plugins/steps/jquery.steps.css" rel="stylesheet">
    <link href="css/plugins/jasny/jasny-bootstrap.min.css" rel="stylesheet">

    <!--min-height: 440px !important;-->

    <!--Custom Style-->
    <style>
        .wizard-big.wizard > .content {
            min-height: 540px !important;
        }
    </style>

</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="midBody" runat="Server">


    <div class="row">
        <div class="col-lg-12">
            <div class="ibox">
                <div class="ibox-title">
                    <h5>Wizard for Similarity Join Query</h5>
                    <div class="ibox-tools">
                        <a class="collapse-link">
                            <i class="fa fa-chevron-up"></i>
                        </a>
                        <a class="close-link">
                            <i class="fa fa-times"></i>
                        </a>
                    </div>
                </div>
                <div class="ibox-content">
                    <h2>Welcome to query group via similarity join.
                    </h2>
                    <p>
                        Please follow the wizard step by step.
                    </p>

                    <form id="form" action="#" class="wizard-big">
                        <h1>Upload Query Sample</h1>
                        <fieldset>
                            <h2>Upload a text file</h2>
                            <div class="row">
                                <div class="col-lg-8">

                                    <div class="form-group">
                                        <label>Only txt file</label>

                                        <div class="ibox float-e-margins">
                                            <div class="fileinput fileinput-new input-group" data-provides="fileinput">
                                                <div class="form-control" data-trigger="fileinput"><i class="glyphicon glyphicon-file fileinput-exists"></i><span id="id_file_name" class="fileinput-filename"></span></div>
                                                <span class="input-group-addon btn btn-default btn-file"><span class="fileinput-new">Select file</span><span class="fileinput-exists">Change</span><input type="file" name="file"></span>
                                                <a href="#" class="input-group-addon btn btn-default fileinput-exists" data-dismiss="fileinput">Remove</a>
                                            </div>
                                        </div>

                                        <p>
                                            Please ensure that the file format is acceptable.
                                        </p>

                                        <p>
                                        </p>
                                    </div>
                                </div>
                                <div class="col-lg-4">
                                    <div class="text-center">
                                        <div style="margin-top: 20px">
                                            <i class="fa fa-sign-in" style="font-size: 180px; color: #e5e5e5"></i>
                                        </div>
                                    </div>
                                </div>
                            </div>

                        </fieldset>
                        <h1>Select Strategy and Parameters</h1>
                        <fieldset>
                            <h2>Profile Information</h2>
                            <div class="row">
                                <div class="col-lg-6">
                                    <div class="form-group">
                                        <label>Strategy *</label>
                                        <select id="select-strategy" class="form-control m-b">
                                            <option value="1">Strategy I</option>
                                            <option value="2">Strategy II</option>
                                            <option value="3">Strategy III</option>
                                        </select>
                                        
                                            If Strategy III, selfQueryR is 1.1
                                    </div>
                                    <div class="form-group">
                                        <label>Thresholds K *</label>
                                        <input id="id_thresholds" type="text" class="form-control required" value="88" />
                                    </div>
                                    <a href="#" onclick="submit_join();" class="input-group-addon btn btn-default fileinput-exists" style="width: 200px;">Submit Join</a>
                                </div>

                            </div>
                        </fieldset>

                        <h1>Accuracy</h1>
                        <fieldset>
                            <h2>Comparing with the querys sample, the distribution is showed as the following figure.</h2>
                            <div class="col-lg-8">
                                <div id="morris-bar-chart"></div>
                            </div>

                        </fieldset>

                        <h1>Performance</h1>
                        <fieldset>
                            <h2>The following table list the time cost of the specific strategy.</h2>
                            <div class="col-lg-8">
                                <table class="table table-bordered">
                                    <thead>
                                        <tr>
                                            <th>#</th>
                                            <th>Process Name</th>
                                            <th>Time Cost</th>
                                            <th>Percent of all</th>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <tr>
                                            <td>1</td>
                                            <td>Self Query</td>
                                            <td id="selfQueryValue">0.2ms</td>
                                            <td id="selfQueryPercent">0.8%</td>
                                        </tr>
                                        <tr>
                                            <td>2</td>
                                            <td>Process LSH</td>
                                            <td id="processLSHValue">6ms</td>
                                            <td id="processLSHPercent">24.8%</td>
                                        </tr>
                                        <tr>
                                            <td>3</td>
                                            <td>Generate Token</td>
                                            <td id="generateTokenValue">10ms</td>
                                            <td id="generateTokenPercent">41.3%</td>
                                        </tr>
                                        <tr>
                                            <td>4</td>
                                            <td>Query Process</td>
                                            <td id="queryProcessValue">8ms</td>
                                            <td id="queryProcessPercent">33%</td>
                                        </tr>
                                    </tbody>
                                </table>
                            </div>

                        </fieldset>
                    </form>
                </div>
            </div>
        </div>

    </div>


</asp:Content>
<asp:Content ID="Content3" ContentPlaceHolderID="botBody" runat="Server">

    <script src="js/plugins/pace/pace.min.js"></script>
    <script src="js/plugins/steps/jquery.steps.min.js"></script>
    <script src="js/plugins/validate/jquery.validate.min.js"></script>
    <script src="js/plugins/jasny/jasny-bootstrap.min.js"></script>
    <script src="js/plugins/ajaxfileupload/ajaxfileupload.js"></script>

    <!-- Morris -->
    <script src="js/plugins/morris/raphael-2.1.0.min.js"></script>
    <script src="js/plugins/morris/morris.js"></script>

    <script src="js/query.js"></script>

</asp:Content>

