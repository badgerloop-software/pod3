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

const PASSWORD = "The Amp Hour Podcast";  //Super secure, just change the password here

function verificationRequest(e) {
    if (popup.style.display === "none") {
        e.stopPropagation();
        popup.style.display = "";
    }
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

