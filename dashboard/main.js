const electron = require('electron');
const app = electron.app;
const BrowserWindow = electron.BrowserWindow;
const path = require('path');
const url = require('url');
let mainWindow;

function createWindow() {
    mainWindow = new BrowserWindow({
        width: 1650,
        height: 1024,
        minWidth: 815,
        center: true,
        frame: false,
        minHeight:600,
        modal: true,
        icon: path.join(__dirname,'/assets/icons/png/64x64new.png'),
        show: false,
    });

    mainWindow.loadURL(url.format({
        pathname: path.join(__dirname, 'index.html'),
        protocol: 'file:',
        slashes: true,
    }));

    mainWindow.on('ready-to-show', () => {
        mainWindow.show();
        mainWindow.focus();
    });

    mainWindow.on('closed', function () {
        mainWindow = null;
    });
}


app.on('ready', () => {
    createWindow();
});

app.on("window-all-closed", function () {
    app.quit();
});
