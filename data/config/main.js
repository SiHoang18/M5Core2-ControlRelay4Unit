const NUMBERPORTS = 48;
const elements = [
    "Name Device", "WiFi SSID", "WiFi Password"
];
const ports = [
    "LED_PORT", "NEOPIXEL_PORT", "RGB_PORT", "LIGHT_PORT", "MOISTURE_PORT",
    "RELAY_PORT", "FAN_PORT", "ULTRASONIC_TRIG_PORT", "ULTRASONIC_ECHO_PORT",
    "MY_SCL", "MY_SDA"
];
const gpioOptions = Array.from({ length: NUMBERPORTS }, (_, i) => `${i + 1}`);
const container = document.getElementsByClassName("container")[0];
const configTab = container.querySelector("#config");
const socket = new WebSocket(`ws://192.168.4.1/ws`);

const inputs = [];         
const portSelectors = [];  

function createTitle() {
    const title = document.createElement("h2");
    title.textContent = "ESP32 Configuration";
    configTab.appendChild(title);
}

function createTextInputs() {
    elements.forEach(element => {
        const tab = document.createElement("div");
        tab.className = "text-input";
        const label = document.createElement("label");
        label.textContent = `${element}: `;
        const input = document.createElement("input");
        input.type = "text";
        tab.appendChild(label);
        tab.appendChild(input);
        configTab.appendChild(tab);
        inputs.push({ key: element, input });
    });
}

function createPortSelectors() {
    ports.forEach(port => {
        const tab = document.createElement("div");
        tab.className = "port-row";

        const label = document.createElement("label");
        label.textContent = `${port}: `;
        label.className = "port-label";

        const select = document.createElement("select");
        select.className = "port-select";

        gpioOptions.forEach(gpio => {
            const option = document.createElement("option");
            option.value = gpio;
            option.textContent = gpio;
            select.appendChild(option);
        });

        tab.appendChild(label);
        tab.appendChild(select);
        configTab.appendChild(tab);
        portSelectors.push({ key: port, select });
    });
}

function createSaveButton() {
    const button = document.createElement("button");
    button.textContent = "Save";

    button.addEventListener("click", () => {
        const data = {};
        let hasEmpty = false;

        inputs.forEach(({ key, input }) => {
            data[key] = input.value.trim();
            if (input.value.trim() === "" && key !== "WiFi Password") {
                hasEmpty = true;
            }
        });

        // portSelectors.forEach(({ key, select }) => {
        //     data[key] = select.value;
        // });

        if (hasEmpty) {
            alert("Please fill in all text fields.");
            return;
        }

        console.log(JSON.stringify(data, null, 2));

        if (socket.readyState === WebSocket.OPEN) {
            socket.send(JSON.stringify(data));
            inputs.forEach(({ input }) => input.value = "");
            // alert("Data Sent");
        } else {
            alert("WebSocket not connected.");
        }
    });

    configTab.appendChild(button);
}

function initConfigUI() {
    createTitle();
    createTextInputs();
    // createPortSelectors();
    createSaveButton();
}

document.addEventListener("DOMContentLoaded", initConfigUI);
