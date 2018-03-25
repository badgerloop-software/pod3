
const charts = require("chart.js");
const moment = require("moment");
// Update X as the t value
// Update Y as value at that t value

function makeChart (canvas, data, name) {
    const chart = new charts.Chart(canvas, {
        type: 'line',
        data: {
            labels: [name],
            datasets: [{
                label: name,
                data:[data],
                lineTension: 0,
                backgroundColor: [
                    'rgba(255, 99, 132, 0.2)',
                ],
                borderColor: [
                    'rgba(255,99,132,1)',
                ],
                borderWidth: 1,
                fill: false,
            }]
        },
        options: {
            /*///////For increasing performance//////
            animation: {
                duration: 0, // general animation time
            },
            hover: {
                animationDuration: 0, // duration of animations when hovering an item
            },
            responsiveAnimationDuration: 0, // animation duration after a resize
            //*////////End performance boosters, comment out above code to make the graph nicer + slower////
            tooltips: {
                enabled: false
            },
            scales: {
                xAxes: [{
                    type: "time",
                    time: {
                        displayFormats: {
                            second: "ss",
                            millisecond: "ss.SSS",
                        },
                        min: new Date(),
                        stepSize: 10,
                    },

                }],
                yAxes:[{
                    ticks: {
                        min: 0,
                        max: 100
                    }

                }]
            }

        }
    });
    console.log(chart.options.scales.xAxes.min);
    module.exports.chart = chart;
}

/**@name addData
 * @purpose Takes in a label for the new data and some new information to
 * add to the graph and updates the graph with that info
 *
 * @param label the tag for the new piece of data (might go away)
 * @param data the actual data point, should be in JSON form e.g. { x:number, y:number }
 *
 **/
//Todo Make options to get rid of stale data
function addData(chart, label, data) {      //fixme Tooltips behave weird, figure out why
    //newChart.data.labels.push(label);
    console.log("TIME 1:" + moment().millisecond());
    let numDataPoints = chart.data.datasets[0].data.length;
    if (numDataPoints >= 10){   //removes the data that is older than 10 iterations, reusable, might be moved to a view change option
        chart.data.datasets[0].data.shift();
        chart.options.scales.xAxes[0].time.min = chart.data.datasets[0].data[0];    //shifts the leftmost point over one
    }
    chart.data.datasets.forEach((dataset) => {
        dataset.data.push(data);
    })
    chart.update(); //may need to handle tooltip glitch here
    console.log("TIME 2:" + moment().millisecond());
}
module.exports.addData = addData;
module.exports.makeChart = makeChart;