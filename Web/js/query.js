
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


function upload_file() {



}

function submit_join() {

    var name = document.getElementById('id_file_name').innerHTML;

    var strategy = $('#select-strategy').val();
    var thresholds = $('#id_thresholds').val();
    $.post("API/GetIndexJoinResult.aspx",
        {
            FilePath: "D:\\" + name,
            JoinType: strategy,
            ThresholdK: thresholds,
            SelfQueryR: 1.1
        }
        , function (result) {


            $.post("API/GetInfoByID.aspx",
            {
                JsonIdList: result,
                Type: 3
            }
            , function (res_distribution) {

                var obj_res_distribution = JSON.parse(res_distribution);


                $.post("API/GetInfoByData.aspx",
                {
                    FilePath: "D:\\" + name,
                    Type: 2
                }
                , function (src_distribution) {

                    var obj_src = JSON.parse(src_distribution);
                    var mapDistribution = { A: 0, B: 0, C: 0, D: 0 };
                    for (var i = 0; i < obj_src.UserTypes.length; i++) {
                        mapDistribution[obj_src.UserTypes[i]]++;
                    }


                    Morris.Bar({
                        element: 'morris-bar-chart',
                        data: [
                            { y: 'VIP-1', a: mapDistribution["A"], b: obj_res_distribution["Numbers"][0] },
                            { y: 'VIP-2', a: mapDistribution["B"], b: obj_res_distribution["Numbers"][1] },
                            { y: 'VIP-3', a: mapDistribution["C"], b: obj_res_distribution["Numbers"][2] },
                            { y: 'VIP-4', a: mapDistribution["D"], b: obj_res_distribution["Numbers"][3] }
                        ],
                        xkey: 'y',
                        ykeys: ['a', 'b'],
                        labels: ['Query', 'Result'],
                        hideHover: 'auto',
                        resize: true,
                        barColors: ['#1ab394', '#8a8a8a'],
                    });


                    $.post("API/GetQueryTime.aspx",
                    {
                    }
                    , function (time_res) {

                        var obj_time = JSON.parse(time_res);

                        selfQuery = obj_time["SelfQuery"];
                        ComputeLSH = obj_time["ComputeLSH"];
                        ComputeToken = obj_time["ComputeToken"];
                        QueryProcess = obj_time["Query"] - selfQuery - ComputeLSH - ComputeToken;

                        AllTime = selfQuery + ComputeLSH + ComputeToken + QueryProcess;


                        document.getElementById('selfQueryValue').innerHTML = selfQuery;
                        document.getElementById('selfQueryPercent').innerHTML = Math.floor(selfQuery * 10000 / AllTime) / 100 + "%";

                        document.getElementById('processLSHValue').innerHTML = ComputeLSH;
                        document.getElementById('processLSHPercent').innerHTML = Math.floor(ComputeLSH * 10000 / AllTime) / 100 + "%";

                        document.getElementById('generateTokenValue').innerHTML = ComputeToken;
                        document.getElementById('generateTokenPercent').innerHTML = Math.floor(ComputeToken * 10000 / AllTime) / 100 + "%";

                        document.getElementById('queryProcessValue').innerHTML = QueryProcess;
                        document.getElementById('queryProcessPercent').innerHTML = Math.floor(QueryProcess * 10000 / AllTime) / 100 + "%";


                    });




                });



            });


        });

}

