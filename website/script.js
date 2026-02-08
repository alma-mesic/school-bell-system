// BURGER MENU
document.getElementById("burger").onclick = function() {
    let menu = document.getElementById("burgerMenu");
    menu.classList.toggle("show");
};

// CARD NAVIGACIJA
const homePage = document.getElementById('homePage');

const pages = {
    notificationsCard: document.getElementById('notificationsPage'),
    scheduleCard: document.getElementById('schedulePage'),
    dutyCard: document.getElementById('dutyPage'),
    testBellCard: document.getElementById('testBellPage')
};

// klik na card
Object.keys(pages).forEach(cardId => {
    document.getElementById(cardId).addEventListener('click', () => {
        homePage.style.display = 'none';
        pages[cardId].classList.add('active');
    });
});

// BACK BUTTONS
document.querySelectorAll('.backBtn').forEach(btn => {
    btn.addEventListener('click', () => {
        document.querySelectorAll('.subPage').forEach(p => p.classList.remove('active'));
        homePage.style.display = 'flex';
    });
});

// TEST ZVONA BUTTONS
document.getElementById('startBell').addEventListener('click', () => {
    // TODO: slanje JSON preko WebSocket ili HTTP request ka ESP32
    document.getElementById('bellStatus').innerText = 'Zvono počelo!';
});

document.getElementById('stopBell').addEventListener('click', () => {
    // TODO: slanje JSON
    document.getElementById('bellStatus').innerText = 'Zvono zaustavljeno!';
});

// Placeholder za dinamičko popunjavanje obavještenja i rasporeda
function loadNotifications() {
    const notificationsContent = document.getElementById('notificationsContent');
    notificationsContent.innerHTML = "<p>Ovdje će se prikazivati obavještenja.</p>";
}
function loadSchedule() {
    const scheduleContent = document.getElementById('scheduleContent');
    scheduleContent.innerHTML = "<p>Ovdje će se prikazivati raspored zvonjenja.</p>";
}
function loadDuty() {
    const dutyContent = document.getElementById('dutyContent');
    dutyContent.innerHTML = "<p>Ovdje će se prikazivati dežurstvo.</p>";
}

// INIT
loadNotifications();
loadSchedule();
loadDuty();
