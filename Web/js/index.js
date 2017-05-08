function initIndexMetaData() {

    $.get("API/GetIndexMetaData.aspx", function (result) {
        var res = JSON.parse(result);
        $('#index_current_number').val(res.IndexCurrentNum);

        $('#index_total_number').val(res.IndexTotalNum);
        $('#index_usable_number').val(res.IndexTotalNum - res.IndexCurrentNum);
        $('#index_load').val(res.IndexLoad + "%");
        $('#index_memory_size').val(res.IndexMemorySize);
        $('#index_memory_use_percent').val(res.MemoryUsePercent + "%");

    });

}

function initIndexDistribution() {

    $.get("API/GetIndexDistribution.aspx", function (result) {
        var res = JSON.parse(result);

        var data = [{
            label: "VIP 1",
            data: 25,
            color: "#d3d3d3",
        }, {
            label: "VIP 2",
            data: 25,
            color: "#bababa",
        }, {
            label: "VIP 3",
            data: 25,
            color: "#79d2c0",
        }, {
            label: "VIP 4",
            data: 25,
            color: "#1ab394",
        }];

        for (var i = 0; i < 4; i++) {
            data[i].label = res.Labels[i];
            data[i].data = res.Numbers[i];
        }

        var plotObj = $.plot($("#index_user_kind_distribution_chart"), data, {
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

}



