
const terminalInput = document.getElementById("terminal-input");
const terminalText = document.getElementById("terminal-text");
const submitBtn = document.getElementById("submit");


terminalInput.onkeydown = function () {
    if (event.keyCode === 13){
        submit();
    }
}


submitBtn.addEventListener("click", submit);

function submit(){
    let input = terminalInput.value;
    //Checking to make sure something  was typed in so there arent invisible line breaks
    if (input === "" || input === "$"){
        return;
    }
    //$ is just for the fans of Bash
    if (input.charAt(0) === '$'){
        input = input.slice(1,input.length);
    }

    terminalText.innerHTML += input + "<br>" + "> ";   //Add input to console
    processText(input)
    terminalInput.value = "$";   //clear submit box
}


function processText(input) {
    input = input.toLowerCase();    //Make it case insensitive
    if (input === "clear") {
        //Clears the entire window in case the commands become too numerous
        terminalText.innerHTML = "> ";
    }

    if (input === "connect") {
        //TODO Make this command establish a connection with the Pi!
        return;
    }

    if (input === "loghistory") {
        //TODO Save a txt file with all of the contents of the terminal text
        return;
    }
}