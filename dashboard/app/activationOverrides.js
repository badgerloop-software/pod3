//TODO Add functionality to the activation override panel
const hv_enable = document.getElementById("hv_enable");
const hv_disable = document.getElementById("hv_disable");
let myNotification;

hv_enable.addEventListener("click", ()=> {
    myNotification = new Notification('Title', {
        body: 'Lorem Ipsum Dolor Sit Amet'

    });
    myNotification.onclick = () => {
        console.log('Notification clicked')
    };

});