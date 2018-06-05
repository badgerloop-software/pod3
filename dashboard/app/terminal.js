const terminalInput = document.getElementsByClassName("terminal-input")[0];
const terminalText = document.getElementById("terminal-text");
const submitBtn = document.getElementById("submit");
const communication = require("./app/communication");

const eventNameBase = "messageReceived_";
defaultIP = "localhost";
defaultPort = 8008;
function printResponse(response) {
    terminalText.innerHTML +=
        "<br>" +
        response +
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

function processText(input, optionalArgs) {
    input = input.toLowerCase();    //Make it case insensitive
    if (input === "help") {
        terminalText.innerHTML += "<br>" +
            " &emsp; clear : Clears the entire window" + "<br>" +
            " &emsp; connect [OPTIONAL ARGS: i=[IP address] p=[Port]] : Checks for a response from the server" + "<br>" +
            " &emsp; configure [configFileName.*] : Sends the designated config file to the server" +
            "<br> > ";
    }

    if (input === "configure") {
        //Add checking for if file exists
        communication.sendMessage("config", defaultIP, defaultPort, "config", optionalArgs[0]);
    }

    if (input === "clear") {
        //Clears the entire window in case the commands become too numerous
        terminalText.innerHTML = "> ";
    }

    if (input === "connect") {
        //Make this command establish a connection with the Pi!
        let newIP = defaultIP;
        let newPort = defaultPort;
        for (let i = 0; i < optionalArgs.length; i++){
            if (optionalArgs[i].charAt(0) === "i") {
                newIP = optionalArgs[i].slice(2);   //cuts off the
            } else if (optionalArgs[i].charAt(0) === "p") {
                newPort = optionalArgs[i].slice(2);
            }
        }
        console.log("IP: " + newIP + "   PORT: " + newPort);
        communication.sendMessage("connect", newIP, newPort);
        return;
    }

    if (input === "query") {
        //TODO allow manual querying of the database, could use second prompt and password verification
        return;
    }

}
