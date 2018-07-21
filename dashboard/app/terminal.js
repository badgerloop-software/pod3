const terminalInput = document.getElementsByClassName("terminal-input")[0];
const termTerminalInput = document.getElementById("term-terminal-input"); /* the terminal input on the big terminal */
const terminalText = document.getElementById("terminal-text");
const termTerminalText = document.getElementById("term-terminal-text"); /* ther terminal text box for the big terminal */
const submitBtn = document.getElementById("submit");
const termSubmitBtn = document.getElementById("term-submit");
const communication = require("./app/communication");

const mainDashDiv = document.getElementById("main-dashboard-div");
const termDashDiv = document.getElementById("terminal-dashboard-div");
const termPageBtn = document.getElementById("terminal-page-button");

const solenoids = [
    document.getElementById("solenoid-1"),
    document.getElementById("solenoid-2"),
    document.getElementById("solenoid-3"),
    document.getElementById("solenoid-4"),
    document.getElementById("solenoid-5")
];
console.log(solenoids);
if (!solenoids.includes(null)) {
    solenoids.forEach((solenoid) => {
        communication.updater.on(solenoid.id + "_activate", () => {
            solenoid.style.background = "green";
        });
        communication.updater.on(solenoid.id + "_deactivate", () => {
            solenoid.style.background = "red";
        })
    });
    solenoids.forEach((solenoid) => {
        solenoid.addEventListener("click", () =>
            solenoid.style.background === "red" ?
                solenoid.style.background = "green" :  solenoid.style.background = "red");
    });
}


const eventNameBase = "messageReceived_";
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

termSubmitBtn.addEventListener("click", termSubmit);

///////Show/Hide Big Console vs Main Dashboard////////
termPageBtn.addEventListener("click", swapDashboard);

function swapDashboard() {
    let onMain = (termDashDiv.style.display === "none");
    if (onMain) {
	// swap the visibility of the dashboard tabs
	mainDashDiv.style.display = "none";
	termDashDiv.style.display = "block";
	termPageBtn.innerHTML = "Main";
    }
    else {
        // swap the visibility of dashboard tabs
	mainDashDiv.style.display = "block";
	termDashDiv.style.display = "none";
	termPageBtn.innerHTML = "Terminal";
    }
}

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

    termTerminalText.innerHTML += fullInput + "<br>" + "> ";   //Add input to big console
    terminalText.innerHTML += fullInput + "<br>" + "> ";   //Add input to console
    processText(input, inputArgs);
    termTerminalInput.value = "";   //clear big console submit box
    terminalInput.value = "";   //clear submit box
}

/* stupid twin function to do the same terminal stuff for the big hidden box */
function termSubmit(){
    let fullInput = termTerminalInput.value;
    let inputArgs = fullInput.split(" ");
    let input = inputArgs[0];
    inputArgs.shift();

    //Checking to make sure something  was typed in so there arent invisible line breaks
    if (input === ""){
        return;
    }

    termTerminalText.innerHTML += fullInput + "<br>" + "> ";   //Add input to big console
    terminalText.innerHTML += fullInput + "<br>" + "> ";   //Add input to console
    processText(input, inputArgs);
    termTerminalInput.value = "";   //clear big console submit box
    terminalInput.value = "";   //clear submit box
}


const solenoidStates = ["_activate", "_deactivate"];
function processText(input, optionalArgs) {
    input = input.toLowerCase();    //Make it case insensitive
    if (input === "help") {
        terminalText.innerHTML += "<br>" +
            " &emsp; clear : Clears the entire window" + "<br>" +
            " &emsp; connect [OPTIONAL ARGS: i=[IP address] p=[Port]] : Checks for a response from the server" + "<br>" +
            " &emsp; set_pod_address [OPTIONAL ARGS: i=[IP address] p=[Port]] : Set the address to talk to the pod" + "<br>" +
            " &emsp; test_solenoids: Randomly flips the solenoids in the diagram (NOT REAL ONES) to test functionality" + "<br>" +
            " &emsp; configure [configFileName.*] : Sends the designated config file to the server" +
            "<br> > ";
        termTerminalText.innerHTML += "<br>" +
            " &emsp; clear : Clears the entire window" + "<br>" +
            " &emsp; connect [OPTIONAL ARGS: i=[IP address] p=[Port]] : Checks for a response from the server" + "<br>" +
            " &emsp; set_pod_address [OPTIONAL ARGS: i=[IP address] p=[Port]] : Set the address to talk to the pod" + "<br>" +
            " &emsp; test_solenoids: Randomly flips the solenoids in the diagram (NOT REAL ONES) to test functionality" + "<br>" +
            " &emsp; configure [configFileName.*] : Sends the designated config file to the server" +
            "<br> > ";
    }

    if (input === "configure") {
        //Add checking for if file exists
	let podIP = communication.getPodIP();
	let podPort = communication.getPodPort();
        communication.sendMessage("config", podIP, podPort, "config", optionalArgs[0]);
    }

    if (input === "test_solenoids") {
        communication.updater.emit("solenoid-1" + solenoidStates[Math.floor(Math.random() * solenoidStates.length)]);
        communication.updater.emit("solenoid-2" + solenoidStates[Math.floor(Math.random() * solenoidStates.length)]);
        communication.updater.emit("solenoid-3" + solenoidStates[Math.floor(Math.random() * solenoidStates.length)]);
        communication.updater.emit("solenoid-4" + solenoidStates[Math.floor(Math.random() * solenoidStates.length)]);
        communication.updater.emit("solenoid-5" + solenoidStates[Math.floor(Math.random() * solenoidStates.length)]);
    }

    if (input === "clear") {
        //Clears the entire window in case the commands become too numerous
        terminalText.innerHTML = "> ";
	termTerminalText.innerHTML = "> ";
    }

    if (input === "connect") {
        //Make this command establish a connection with the Pi!
        let newIP = communication.getServerIP();
        let newPort = communication.getServerPort();
        for (let i = 0; i < optionalArgs.length; i++){
            if (optionalArgs[i].charAt(0) === "i") {
                newIP = optionalArgs[i].slice(2);   //cuts off the
            } else if (optionalArgs[i].charAt(0) === "p") {
                newPort = optionalArgs[i].slice(2);
            }
        }
	communication.setServerIP(newIP);
	communication.setServerPort(newPort);
        console.log("IP: " + newIP + "   PORT: " + newPort);
        communication.sendMessage("connect", newIP, newPort);
        return;
    }

    if (input === "set_pod_address") {
        let newIP = communication.getPodIP();
        let newPort = communication.getPodPort();
        for (let i = 0; i < optionalArgs.length; i++){
            if (optionalArgs[i].charAt(0) === "i") {
                newIP = optionalArgs[i].slice(2); //cuts off the i=
            } else if (optionalArgs[i].charAt(0) === "p") {
                newPort = optionalArgs[i].slice(2); // cuts off the p=
            }
        }
        // change our current state for pod communication
        communication.setPodIP(newIP);
        communication.setPodPort(newPort);
        console.log("changing pod adress to " + newIP + ":" + newPort);
    }

    if (input === "query") {
        //TODO allow manual querying of the database, could use second prompt and password verification
        return;
    }

}
