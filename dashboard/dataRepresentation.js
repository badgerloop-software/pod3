
const charts = require("chart.js");

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
            scales: {
                xAxes: [{
                    type: 'time',
                    time:{
                        unit:"millisecond"
                    },
                    distribution: 'series',

                    ticks: {
                        beginAtZero: true,
                        stepSize: .5
                    },
                }],
                yAxes: [{
                    type: 'linear',
                    ticks: {
                        beginAtZero:true
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
function addData(chart, label, data) {
    //newChart.data.labels.push(label);
    chart.data.datasets.forEach((dataset) => {
        dataset.data.push(data);
    })
    chart.update();
}
module.exports.addData = addData;
module.exports.makeChart = makeChart;