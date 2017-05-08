<%@ Page Title="" Language="C#" MasterPageFile="~/Admin.master" AutoEventWireup="true" CodeFile="query.aspx.cs" Inherits="query" %>

<asp:Content ID="Content1" ContentPlaceHolderID="botHead" runat="Server">

    <link href="css/plugins/steps/jquery.steps.css" rel="stylesheet">
    <link href="css/plugins/jasny/jasny-bootstrap.min.css" rel="stylesheet">

    <!--Custom Style-->
    <style>
        .wizard-big.wizard > .content {
            min-height: 440px !important;
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
                                                <div class="form-control" data-trigger="fileinput"><i class="glyphicon glyphicon-file fileinput-exists"></i><span class="fileinput-filename"></span></div>
                                                <span class="input-group-addon btn btn-default btn-file"><span class="fileinput-new">Select file</span><span class="fileinput-exists">Change</span><input type="file" name="..."></span>
                                                <a href="#" class="input-group-addon btn btn-default fileinput-exists" data-dismiss="fileinput">Remove</a>
                                            </div>
                                        </div>

                                        <p>
                                            Please ensure that the file format is acceptable.
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
                                        <select class="form-control m-b">
                                            <option>Strategy I</option>
                                            <option>Strategy II</option>
                                            <option>Strategy III</option>
                                        </select>
                                    </div>
                                    <div class="form-group">
                                        <label>Thresholds K *</label>
                                        <input type="text" class="form-control required">
                                    </div>
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
                                            <td>0.2ms</td>
                                            <td>0.8%</td>
                                        </tr>
                                        <tr>
                                            <td>2</td>
                                            <td>Process LSH</td>
                                            <td>6ms</td>
                                            <td>24.8%</td>
                                        </tr>
                                        <tr>
                                            <td>3</td>
                                            <td>Generate Token</td>
                                            <td>10ms</td>
                                            <td>41.3%</td>
                                        </tr>
                                        <tr>
                                            <td>4</td>
                                            <td>Query Process</td>
                                            <td>8ms</td>
                                            <td>33%</td>
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

    <!-- Morris -->
    <script src="js/plugins/morris/raphael-2.1.0.min.js"></script>
    <script src="js/plugins/morris/morris.js"></script>

    <script>
        $(document).ready(function () {
            $("#wizard").steps();
            $("#form").steps({
                bodyTag: "fieldset",
                onStepChanging: function (event, currentIndex, newIndex) {
                    // Always allow going backward even if the current step contains invalid fields!
                    if (currentIndex > newIndex) {
                        return true;
                    }

                    // Forbid suppressing "Warning" step if the user is to young
                    if (newIndex === 3 && Number($("#age").val()) < 18) {
                        return false;
                    }

                    var form = $(this);

                    // Clean up if user went backward before
                    if (currentIndex < newIndex) {
                        // To remove error styles
                        $(".body:eq(" + newIndex + ") label.error", form).remove();
                        $(".body:eq(" + newIndex + ") .error", form).removeClass("error");
                    }

                    // Disable validation on fields that are disabled or hidden.
                    form.validate().settings.ignore = ":disabled,:hidden";

                    // Start validation; Prevent going forward if false
                    return form.valid();
                },
                onStepChanged: function (event, currentIndex, priorIndex) {
                    // Suppress (skip) "Warning" step if the user is old enough.
                    if (currentIndex === 2 && Number($("#age").val()) >= 18) {
                        $(this).steps("next");
                    }

                    // Suppress (skip) "Warning" step if the user is old enough and wants to the previous step.
                    if (currentIndex === 2 && priorIndex === 3) {
                        $(this).steps("previous");
                    }
                },
                onFinishing: function (event, currentIndex) {
                    var form = $(this);

                    // Disable validation on fields that are disabled.
                    // At this point it's recommended to do an overall check (mean ignoring only disabled fields)
                    form.validate().settings.ignore = ":disabled";

                    // Start validation; Prevent form submission if false
                    return form.valid();
                },
                onFinished: function (event, currentIndex) {
                    var form = $(this);

                    // Submit form input
                    form.submit();
                }
            }).validate({
                errorPlacement: function (error, element) {
                    element.before(error);
                },
                rules: {
                    confirm: {
                        equalTo: "#password"
                    }
                }
            });
        });



        Morris.Bar({
            element: 'morris-bar-chart',
            data: [
                { y: 'VIP-1', a: 60, b: 50 },
                { y: 'VIP-2', a: 75, b: 65 },
                { y: 'VIP-3', a: 50, b: 40 },
                { y: 'VIP-4', a: 75, b: 65 }
            ],
            xkey: 'y',
            ykeys: ['a', 'b'],
            labels: ['Query', 'Result'],
            hideHover: 'auto',
            resize: true,
            barColors: ['#1ab394', '#cacaca'],
        });




    </script>

</asp:Content>

