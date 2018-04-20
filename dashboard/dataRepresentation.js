
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
                    '#540306',
                ],
                borderColor: [
                    '#540306',
                ],
                borderWidth: 1,
                fill: false,
            }]
        },
        options: {
            ///////For increasing performance//////
            animation: {
                duration: 0, // general animation time
            },
            hover: {
                animationDuration: 0, // duration of animations when hovering an item
            },
            responsiveAnimationDuration: 0, // animation duration after a resize
            ////////End performance boosters, comment out above code to make the graph nicer + slower////
            tooltips: {
                enabled: true
            },
            scales: {
                xAxes: [{
                    type: "time",
                    time: {
                        displayFormats: {
                            second: "mm:ss.S",
                            millisecond: "mm:ss.S",
                        },
                        min: new Date(),
                        //stepSize: 500,

                    },
                    ticks: {
                        source: 'data'
                    }

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
    let numDataPoints = chart.data.datasets[0].data.length;
    if (numDataPoints >= 10){   //removes the data that is older than 10 iterations, reusable, might be moved to a view change option
        chart.data.datasets[0].data.shift();
        chart.options.scales.xAxes[0].time.min = chart.data.datasets[0].data[0];    //shifts the leftmost point over one
    }
    chart.data.datasets.forEach((dataset) => {
        dataset.data.push(data);
    })
    chart.update(); //may need to handle tooltip glitch here
}
module.exports.addData = addData;
module.exports.makeChart = makeChart;