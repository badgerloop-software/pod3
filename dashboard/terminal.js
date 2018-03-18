
const terminalInput = document.getElementsByClassName("terminal-input")[0];
const terminalText = document.getElementById("terminal-text");
const submitBtn = document.getElementById("submit");
const communication = require("./communication");

//Lets you press enter to submit a key press
terminalInput.addEventListener('keydown',() => {
    if (event.keyCode === 13) {
        submit();
    }
})

//submit when the button is pressed
submitBtn.addEventListener("click", submit);


///////Helper Functions///////
function submit(){
    let input = terminalInput.value;
    //Checking to make sure something  was typed in so there arent invisible line breaks
    if (input === ""){
        return;
    }


    terminalText.innerHTML += input + "<br>" + "> ";   //Add input to console
    processText(input)
    terminalInput.value = "";   //clear submit box
}

//When a message is received from the pi it updates the terminal
communication.updater.on("messageReceived", (data) => {
    console.log(communication.data);
    terminalText.innerHTML += "<br>" +
        data +
        "<br> > ";
})

function processText(input) {
    input = input.toLowerCase();    //Make it case insensitive
    if (input === "help") {
        terminalText.innerHTML += "<br>" +
            " &emsp; clear : Clears the entire window" +
            "<br> > ";
    }



    if (input === "clear") {
        //Clears the entire window in case the commands become too numerous
        terminalText.innerHTML = "> ";
    }

    if (input === "connect") {
        //TODO Make this command establish a connection with the Pi!
        communication.sendMessage();
        return;
    }

    if (input === "loghistory") {
        //TODO Save a txt file with all of the contents of the terminal text
        return;
    }

}