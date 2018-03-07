// Author: Bloop Software
// Brief: This has the top right menu controls
// and other styling choices for the window itself

const electron = require("electron");
const closer = document.getElementById("close");
const minimizer = document.getElementById('minimize');
const maximizer = document.getElementById('maximize');
//const top = document.getElementById('nav_header');




closer.addEventListener('click', (err) => {
    electron.remote.getCurrentWindow().close();
});

minimizer.addEventListener('click', (err) => {
    electron.remote.getCurrentWindow().minimize();
});


let previousSize;

maximizer.addEventListener('click', (err) => {
    if (electron.remote.getCurrentWindow().isMaximized()){
        electron.remote.getCurrentWindow().setSize(previousSize[0], previousSize[1]);
        electron.remote.getCurrentWindow().center();
        maximizer.innerHTML = '&#128470';
    } else {
        previousSize = electron.remote.getCurrentWindow().getSize();
        electron.remote.getCurrentWindow().maximize();
        maximizer.innerHTML = '&#128470;';
    }
});

let newWindow;
const brand = document.getElementById('brand');
brand.addEventListener('click', () => {
    newWindow = new electron.remote.BrowserWindow({
        width: 600,
        height: 600,
        show: false,
        icon: __dirname + '/assets/icons/png/64x64.png'
    })

    newWindow.loadURL('http://badgerloop.com');
    newWindow.on('ready-to-show', () => {newWindow.show()})
    newWindow.on('closed', () => {newWindow = null})
})