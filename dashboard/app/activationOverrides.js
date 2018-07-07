const communication = require("./communication");

const hv_enable = document.getElementById("hv_enable");
const hv_disable = document.getElementById("hv_disable");
const popup = document.getElementById("popup-container");
const whole_window = document.getElementById("page-container");
const password_input = document.getElementById("confirm-window-password");
const confirm_window_btn = document.getElementById("confirm-window-button");
const brake_on = document.getElementById("brake_on");
const brake_off = document.getElementById("brake_off");
const brake_vent_on = document.getElementById("brake_vent_on");
const brake_vent_off = document.getElementById("brake_vent_off");

const PASSWORD = "betsy";  //Super secure, just change the password here

function verificationRequest(e) {
/*    if (popup.style.display === "none") {
        e.stopPropagation();
        popup.style.display = "";
    }*/
    // make sure we really want to issue the command
    isConfirmed = confirmCommand();
    if (!isConfirmed) {
        return;
    }
    console.log("Command confirmed");
    // send a command to the pod if we clicked an override button
    let podIP = communication.getPodIP();
    let podPort = communication.getPodPort();
    if (this === brake_on || this === brake_off) {
	let onoff = (this === brake_on) ? "on" : "off";
        let payload = {"value": onoff};
        communication.postPayload(podIP, podPort, 'prim_brake_onoff', payload);
    }
    else if (this === brake_vent_on || this === brake_vent_off) {
        let onoff = (this === brake_vent_on) ? "on" : "off";
        let payload = {"value": onoff};
        communication.postPayload(podIP, podPort, 'prim_brake_vent', payload);
    }
    else if (this === hv_enable || this === hv_disable) {
        let endis = (this === hv_enable) ? "enable" : "disable";
        let payload = {"value": endis};
        communication.postPayload(podIP, podPort, 'high_voltage', payload);
    }
    // TODO add whatever other buttons we need for overrides
}

function confirmCommand() {
    const {dialog} = require("electron").remote;
    let buttons = ["Cancel", "Confirm"];
    var dialogRes = dialog.showMessageBox({
        type: "none",
        message: "Are you sure you want to send that command?",
        buttons: buttons
    });
    return dialogRes === buttons.indexOf("Confirm");
}

function checkPassword(e) {
    if (password_input.value === PASSWORD) {
        password_input.value = "";
        password_input.placeholder = "Verification Successful";
    } else {
        password_input.value = "";
        password_input.placeholder = "Try again"
    }
}

hv_disable.addEventListener("click", verificationRequest);

hv_enable.addEventListener("click", verificationRequest);

brake_on.addEventListener("click", verificationRequest);

brake_off.addEventListener("click", verificationRequest);

brake_vent_on.addEventListener("click", verificationRequest);

brake_vent_off.addEventListener("click", verificationRequest);

whole_window.addEventListener("click", () => {popup.style.display = "none"});

password_input.addEventListener("keydown", (e)=> {
    if (event.keyCode === 13) {
        checkPassword(e);
    }
});

confirm_window_btn.addEventListener("click", checkPassword);

