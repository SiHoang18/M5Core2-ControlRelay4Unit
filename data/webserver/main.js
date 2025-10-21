const socket = new WebSocket(`ws://${location.host}/ws`);

socket.addEventListener("open", () => {
    console.log("WebSocket connected.");
});

socket.addEventListener("message", (event) => {
    try {
        const data = JSON.parse(event.data); 
        // console.log("Received JSON:", data);

        for (let i = 1; i <= 4; i++) {
            const relayId = `relay_${i}`;         
            const state = (data[relayId] || 'OFF').toUpperCase();
            const card = document.getElementById(`relay_${i}`);
            
            if (card) {
                updateRelayCard(card, state);
            } else {
                console.warn("Không tìm thấy DOM cho", relayId);
            }
        }
    } catch (err) {
        console.error("Invalid JSON:", event.data);
    }
});

function updateRelayCard(card, state) {
    card.dataset.state = state;
    card.querySelector(".relay-state").textContent = state;
    const btn = card.querySelector(".relay-button");
    btn.textContent = state === 'ON' ? "Turn OFF" : "Turn ON";
}

document.querySelectorAll(".relay-button").forEach((btn) => {
    btn.addEventListener("click", () => {
        const card = btn.closest(".relay-card");
        const relayIndex = card.dataset.index;
        const relayId = `relay_${parseInt(relayIndex) + 1}`;

        const currentState = card.dataset.state === "ON" ? "ON" : "OFF";
        const newState = currentState === "ON" ? "OFF" : "ON";

        updateRelayCard(card, newState);

        const msg = JSON.stringify({ [relayId]: newState });
        console.log("Send:", msg);
        socket.send(msg);
        if (socket.readyState === WebSocket.OPEN) {
            socket.send(msg);
        } else {
            console.warn("WebSocket not ready!");
        }
    });
});
