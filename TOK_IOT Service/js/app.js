var timeTicket;
clearInterval(timeTicket);
timeTicket = setInterval(function() {
    $.get("http://47.106.209.211:8888/img", function(data, status) {
        document.getElementById('video1').src = data;
    });
}, 30);




/////////////////////////////////////////////////////开关功能
function checkboxOnclick1(checkbox) { //开关1
    if (checkbox.checked == true) {
        //alert(test);
        var word = "id=1";
        //把word发给后台程序//返回的数据放在data中，返回状态放在status
        $.post("/btn", {
            message: word
        }, function(data, status) {
            if (status == "success") {
                $("#result").html(data);
            } else {
                alert("Ajax 失败");
            }
        });
        //Action for checked
    } else {
        //Action for not checked
        // alert("OFF");
        var word = "id=0";
        //把word发给后台程序//返回的数据放在data中，返回状态放在status
        $.post("/btn", {
            message: word
        }, function(data, status) {
            if (status == "success") {
                $("#result").html(data);
            } else {
                alert("Ajax 失败");
            }
        });
    }
}
////////////////////////////////////////////////////////////Websocket
var host = window.location.host;
var ws = new WebSocket('ws://' + host + '/ws');
var $message = $('#message');

var meg = 0;
var image = 0;
var longitude = 0;

var arr = new Array();

ws.onopen = function() {
    $message.attr("class", 'label label-success');
    $message.text('open');
};

ws.onmessage = function(ev) {
    $message.attr("class", 'label label-info');
    $message.hide();
    $message.fadeIn("slow");
    $message.text('recieved message');

    var json = JSON.parse(ev.data);

    var $rowid = $('#row' + json.id);
    $('#' + json.id).hide();
    $('#' + json.id).fadeIn("slow");
    $('#' + json.id).text(json.value);

    var $rowid = $('#row' + json.id);
    if (json.value > 500) {
        $rowid.attr("class", "error");
    } else if (json.value > 200) {
        $rowid.attr("class", "warning");
    } else {
        $rowid.attr("class", "");
    }

    table_data = json.value; //value值写入echart

    longitude = json.long; //从json获取经度
    latitude = json.lat; //从json获取纬度数据
    addMarker(); //添加地图点

};
ws.onclose = function(ev) {
    $message.attr("class", 'label label-important');
    $message.text('closed');
};
ws.onerror = function(ev) {
    $message.attr("class", 'label label-warning');
    $message.text('error occurred');
};

/////////////////////////////////////////////////////////echart模块
var table_data = 1;

require.config({
    paths: {
        echarts: 'http://echarts.baidu.com/build/dist'
    }
});
// 使用
require
    (
        [
            'echarts',
            'echarts/chart/line',
            'echarts/chart/bar' // 使用柱状图就加载bar模块，按需加载
        ],
        function(ec) {
            // 基于准备好的dom，初始化echarts图表
            var myChart = ec.init(document.getElementById('main'));
            var option = {
                title: {
                    // text: '图形显示',
                    // subtext: 'Powerd by echart'
                },
                tooltip: {
                    trigger: 'axis'
                },
                legend: {
                    data: ['1', '2']
                },
                toolbox: {
                    show: true,
                    feature: {
                        mark: {
                            show: true
                        },
                        dataView: {
                            show: true,
                            readOnly: false
                        },
                        magicType: {
                            show: true,
                            type: ['line', 'bar']
                        },
                        restore: {
                            show: true
                        },
                        saveAsImage: {
                            show: true
                        }
                    }
                },
                dataZoom: {
                    show: true,
                    start: 0,
                    end: 100
                },
                xAxis: [{
                        name: 'Time',
                        type: 'category',
                        boundaryGap: true,
                        data: (function() {
                            var now = new Date();
                            var res = [];
                            var len = 4;
                            while (len--) {
                                res.unshift(now.toLocaleTimeString().replace(/^\D*/, ''));
                                now = new Date(now - 2000);
                            }
                            return res;
                        })()
                    },
                    {
                        type: 'category',
                        boundaryGap: true,
                        data: (function() {
                            var res = [];
                            var len = 300;
                            while (len--) {
                                res.push(len + 1);
                            }
                            return res;
                        })()
                    }
                ],
                yAxis: [{
                        type: 'value',
                    },
                    {
                        type: 'value',
                    }
                ],
                series: [{
                    name: 'Data',
                    type: 'bar',
                    xAxisIndex: 1,
                    yAxisIndex: 1,
                    data: (function() {
                        var res = [];
                        var len = 300;
                        while (len--) {
                            //res.push(Math.round(Math.random() * 1000));
                            res.push(0);
                        }
                        return res;
                    })()
                }, ]
            };;
            myChart.setOption(option);
            var timeTicket;
            clearInterval(timeTicket);
            timeTicket = setInterval(function() {
                // 动态数据接口 addData
                // table_data=table_data+1;
                myChart.addData([
                    [
                        0, // 系列索引
                        {
                            //value:Math.round(Math.random() * 1000), // 新增数据
                            value: table_data,
                            //value: NULL,
                        },
                        false, // 新增数据是否从队列头部插入
                        false // 是否增加队列长度，false则自定删除原有数据，队头插入删队尾，队尾插入删队头
                    ],
                ]);

            }, 5000);
        }
    );

///////////////////////////////////////////////////////////////////////////////////////天气显示模块
var bl = '北京市'; //你要的变量

AMap.plugin('AMap.Weather', function() {
    var weather = new AMap.Weather();
    //查询实时天气信息, 查询的城市到行政级别的城市，如朝阳区、杭州市
    weather.getLive(bl, function(err, data) {

        if (!err) {
            var str = [];
            // str.push('<h4 >实时天气' + '</h4><hr>');
            str.push('<h4>实时天气:</h4>');
            str.push('<p>城市/区：' + data.city + '');
            str.push('<p>天气：' + data.weather + '');
            str.push('<p>温度：' + data.temperature + '℃');
            str.push('<p>风向：' + data.windDirection + '');
            str.push('<p>风力：' + data.windPower + ' 级');
            str.push('<p>空气湿度：' + data.humidity + '');
            str.push('<p>发布时间：' + data.reportTime + '');
            document.getElementById('liveweather').innerHTML = str.join('');
            //weatherAPI=data.city+data.weather+data.temperature+data.windDirection;
        }
    });
    //未来4天天气预报
    weather.getForecast(bl, function(err, data) {
        if (err) { return; }
        var str = [];
        str.push('<h4>未来四天天气:</h4>');
        for (var i = 0, dayWeather; i < data.forecasts.length; i++) {
            dayWeather = data.forecasts[i];
            str.push('<p>' + dayWeather.date + ' <span class="weather">' + dayWeather.dayWeather + '</span> ' + dayWeather.nightTemp + '~' + dayWeather.dayTemp + '℃');
        }
        document.getElementById('forecast').innerHTML = str.join('');
    });
});

//////////////////////////////////////////////////////////////////地图API

var longitude = 116.4269240000;
var latitude = 39.9908660000;
var marker, map = new AMap.Map("container", {
    resizeEnable: true,
    center: [116.397428, 39.90923],
    zoom: 14
});
// var myVar = setInterval(function() {
//   addMarker()
// }, 1000);
function addMarker() {

    map.setZoomAndCenter(15, [longitude, latitude]); //更新中心点

    marker = new AMap.Marker({
        map: map,
        icon: "https://webapi.amap.com/theme/v1.3/markers/n/mark_b.png",
        position: [longitude, latitude],

    });
    marker.setMap(map);
}