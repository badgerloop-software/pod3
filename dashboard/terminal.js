
const terminalInput = document.getElementsByClassName("terminal-input")[0];
const terminalText = document.getElementById("terminal-text");
const submitBtn = document.getElementById("submit");
const communication = require("./communication");
//const fs = require("fs");

const eventNameBase = "messageReceived_";
function printResponse(data) {
    terminalText.innerHTML += "<br>" +
        data +
        "<br> > ";
}

//When a message is received from the pi it updates the terminal
communication.updater.on(eventNameBase + "connect", (data) => {
    printResponse(data);
});
communication.updater.on(eventNameBase + "config", (data) => {
    printResponse(data);
});

//Lets you press enter to submit a key press
terminalInput.addEventListener('keydown',() => {
    if (event.keyCode === 13) {
        submit();
    }
});

//submit when the button is pressed
submitBtn.addEventListener("click", submit);


///////Helper Functions///////
function submit(){
    let fullInput = terminalInput.value;
    let inputArgs = fullInput.split(" ");
    let input = inputArgs[0];
    inputArgs.shift();

    //Checking to make sure something  was typed in so there arent invisible line breaks
    if (input === ""){
        return;
    }

    terminalText.innerHTML += fullInput + "<br>" + "> ";   //Add input to console
    processText(input, inputArgs);
    terminalInput.value = "";   //clear submit box
}

<<<<<<< HEAD
<<<<<<< HEAD
//When a message is received from the pi it updates the terminal
communication.updater.on("messageReceived", (data) => {
    console.log(communication.data);
    terminalText.innerHTML += "<br>" +
        data +
        "<br> > ";
})



function processText(input) {
=======
function processText(input, optionalArgs) {
>>>>>>> f75e16a... Changed the communication to have more robust default parameter handling, set up file transfer framework on dashboard side, made some minor visual tweaks to the dashboard, fixed the embedded terminal issue, added the ability for data to change on the main display based on remote requests
=======
function processText(input, optionalArgs) {
>>>>>>> f75e16a... Changed the communication to have more robust default parameter handling, set up file transfer framework on dashboard side, made some minor visual tweaks to the dashboard, fixed the embedded terminal issue, added the ability for data to change on the main display based on remote requests
    input = input.toLowerCase();    //Make it case insensitive
    if (input === "help") {
        terminalText.innerHTML += "<br>" +
            " &emsp; clear : Clears the entire window" + "<br>" +
            " &emsp; connect [OPTIONAL ARGS: [IP address] [Port]] : Checks for a response from the server" + "<br>" +
            " &emsp; configure [configFileName.*] : Sends the designated config file to the server" +
            "<br> > ";
    }

    if (input === "configure") {
        //Add checking for if file exists
        let RESTArgs = ["config", optionalArgs[0]];
        communication.sendMessage("config", {restParams:RESTArgs});
    }

    if (input === "clear") {
        //Clears the entire window in case the commands become too numerous
        terminalText.innerHTML = "> ";
    }

    if (input === "connect") {
        //TODO Make this command establish a connection with the Pi!
        communication.sendMessage("connect", {ip:optionalArgs[0], port:optionalArgs[1]});
        return;
    }

    if (input === "loghistory") {
        //TODO Save a txt file with all of the contents of the terminal text
        return;
    }

}