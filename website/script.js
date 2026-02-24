/*****************BRGER MENI*********************/
const burger = document.getElementById("burger");
if (burger) {
    burger.onclick = function(){
        let menu = document.getElementById("burger_menu");
        menu.classList.toggle("show");
    }
}

let events = [];

/********DIVS - main**********/

const notificationsCard = document.getElementById("notificationsCard");
if (notificationsCard) {
    notificationsCard.addEventListener("click", function() {
        window.location.href = "obavjestenja.html";
    });
}

const scheduleCard = document.getElementById("scheduleCard");
if (scheduleCard) {
    scheduleCard.addEventListener("click", function() {
        window.location.href = "raspored.html";
    });
}

const dutyCard = document.getElementById("dutyCard");
if (dutyCard) {
    dutyCard.addEventListener("click", function() {
        window.location.href = "dezurstvo.html";
    });
}

const testBellCard = document.getElementById("testBellCard");
if (testBellCard) {
    testBellCard.addEventListener("click", function() {
        window.location.href = "test.html";
    });
}


/*****************RASPORED*********************/

const redovni = [
    {
        cas:1,
        pocetak:"7:30",
        kraj:"8:15"
    },
    {
        cas:2,
        pocetak:"8:20",
        kraj:"9:05"
    },
    {
        cas:3,
        pocetak:"9:10",
        kraj:"9:55"
    },
    {
        cas:4,
        pocetak:"10:15",
        kraj:"11:00"
    },
    {
        cas:5,
        pocetak:"11:05",
        kraj:"11:50"
    },
    {
        cas:6,
        pocetak:"11:55",
        kraj:"12:40"
    },
    {
        cas:7,
        pocetak:"12:45",
        kraj:"13:30"
    },
    {
        cas:8,
        pocetak:"13:35",
        kraj:"14:20"
    },
    {
        cas:9,
        pocetak:"14:25",
        kraj:"15:10"
    },
    {
        cas:10,
        pocetak:"15:30",
        kraj:"16:15"
    },
    {
        cas:11,
        pocetak:"16:20",
        kraj:"17:05"
    },
    {
        cas:12,
        pocetak:"17:10",
        kraj:"17:55"
    },
    {
        cas:13,
        pocetak:"18:00",
        kraj:"18:45"
    }
]

const skraceni = [
    {
        cas:1,
        pocetak:"7:30",
        kraj:"8:00"
    },
    {
        cas:2,
        pocetak:"8:05",
        kraj:"8:35"
    },
    {
        cas:3,
        pocetak:"8:40",
        kraj:"9:10"
    },
    {
        cas:4,
        pocetak:"9:30",
        kraj:"10:00"
    },
    {
        cas:5,
        pocetak:"10:05",
        kraj:"10:35"
    },
    {
        cas:6,
        pocetak:"10:40",
        kraj:"11:10"
    },
    {
        cas:7,
        pocetak:"11:15",
        kraj:"11:45"
    },
    {
        cas:8,
        pocetak:"11:50",
        kraj:"12:20"
    },
    {
        cas:9,
        pocetak:"12:25",
        kraj:"12:55"
    },
    {
        cas:10,
        pocetak:"13:15",
        kraj:"13:45"
    },
    {
        cas:11,
        pocetak:"13:50",
        kraj:"14:20"
    },
    {
        cas:12,
        pocetak:"14:25",
        kraj:"14:55"
    },
    {
        cas:13,
        pocetak:"15:00",
        kraj:"15:30"
    }
]

function kreirajTabelu(podaci, tabelaId) {
    const tabela = document.getElementById(tabelaId);
    tabela.innerHTML = ""; 

    const thead = document.createElement("thead");
    const headerRed = document.createElement("tr");

    const kolone = ["Br. časa", "Početak", "Kraj"];

    kolone.forEach(naziv => {
        const th = document.createElement("th");
        th.textContent = naziv;
        headerRed.appendChild(th);
    });

    thead.appendChild(headerRed);
    tabela.appendChild(thead);

    const tbody = document.createElement("tbody");

    podaci.forEach(casObj => {
        const red = document.createElement("tr");

        const tdCas = document.createElement("td");
        tdCas.textContent = casObj.cas;

        const tdPocetak = document.createElement("td");
        tdPocetak.textContent = casObj.pocetak;

        const tdKraj = document.createElement("td");
        tdKraj.textContent = casObj.kraj;

        red.appendChild(tdCas);
        red.appendChild(tdPocetak);
        red.appendChild(tdKraj);

        tbody.appendChild(red);
    });
    tabela.appendChild(tbody);
}

document.addEventListener("DOMContentLoaded", function() {
    kreirajTabelu(redovni, "redovno");
    kreirajTabelu(skraceni, "skraceno");

    const redRadio = document.querySelector('input[value="redovno"]');
    const skrRadio = document.querySelector('input[value="skraceno"]');

    const redBox = document.getElementById("redovno").parentElement;
    const skrBox = document.getElementById("skraceno").parentElement;

    function updateActiveTable() {
        redBox.classList.remove("active");
        skrBox.classList.remove("active");

        if (redRadio.checked) {
            redBox.classList.add("active");
        } else {
            skrBox.classList.add("active");
        }
    }

    redRadio.addEventListener("change", updateActiveTable);
    skrRadio.addEventListener("change", updateActiveTable);

    updateActiveTable();
});


/*****************TEST ZVONA*********************/
const systemPanel = document.querySelector(".system-panel");
const testZvona = document.getElementById("test_zvona");
document.addEventListener("DOMContentLoaded", function () {

    const statusSpan = document.querySelector("#statusIndicator span");

    if (!testZvona) return;

    let aktivno = false;

    function updateUI() {
        if (aktivno) {
            testZvona.textContent = "Zaustavi";
            testZvona.classList.add("active");
            systemPanel.classList.add("ringing");
            statusSpan.textContent = "AKTIVNO";
            statusSpan.style.color = "#2ecc71";
        } else {
            testZvona.textContent = "Pokreni";
            testZvona.classList.remove("active");
            systemPanel.classList.remove("ringing");
            statusSpan.textContent = "NEAKTIVNO";
            statusSpan.style.color = "#E74C3C";
        }
    }

    testZvona.addEventListener("click", function () {
        aktivno = !aktivno;
        updateUI();
    });

    updateUI(); 
});

/*****************EMERGENCY*********************/
const emergencyBtn = document.querySelector(".emergency-btn");
const burgerBtn = document.getElementById("burger");

if (emergencyBtn) {

    let emergencyActive = false;

    emergencyBtn.addEventListener("click", function () {

        if (!emergencyActive) {

            const potvrda = confirm("Da li ste sigurni da želite aktivirati HITNO ZVONO?");
            if (!potvrda) return;

            emergencyActive = true;
            activateEmergency();

        } 
        else {

            const potvrdaStop = confirm("Da li želite zaustaviti emergency stanje?");
            if (!potvrdaStop) return;

            emergencyActive = false;
            deactivateEmergency();
        }

    });


    function activateEmergency() {

        const sviElementi = document.querySelectorAll("button, a, input");

        sviElementi.forEach(el => {
            if (!el.classList.contains("emergency-btn")) {
                el.disabled = true;
                el.style.pointerEvents = "none";
                el.style.opacity = "0.5";
            }
        });

        emergencyBtn.classList.add("emergency-active");
        systemPanel.classList.add("emergency-active");
        emergencyBtn.textContent = "DEAKTIVIRAJ HITNO";

        if (testZvona) testZvona.disabled = true;
        if (burger) burger.classList.add("blocked");
    }


    function deactivateEmergency() {

        const sviElementi = document.querySelectorAll("button, a, input");

        sviElementi.forEach(el => {
            el.disabled = false;
            el.style.pointerEvents = "";
            el.style.opacity = "";
        });

        emergencyBtn.classList.remove("emergency-active");
        systemPanel.classList.remove("emergency-active");
        emergencyBtn.textContent = "Emergency";

        if (testZvona) testZvona.disabled = false;
        if (burger) burger.classList.remove("blocked");
    }
}


/*********************SETTINGS************************* */
function setupColorPreview(inputId, previewId) {
    const colorInput = document.getElementById(inputId);
    const preview = document.getElementById(previewId);

    preview.style.backgroundColor = colorInput.value;

    colorInput.addEventListener('input', () => {
        preview.style.backgroundColor = colorInput.value;
    });
}

setupColorPreview('letter_color_change', 'letter_color_preview');
setupColorPreview('marix_time_color_change', 'marix_time_color_preview');
setupColorPreview('matrix_letter_color_change', 'matrix_letter_color_preview');

/********************OBAVJEŠTENJA**********************/

function addEvent() {
    const name = document.getElementById("eventName").value;
    const datetime = document.getElementById("datetime").value;

    if (!name || !datetime) {
        alert("Popuni sva polja!");
        return;
    }

    const event = {
        name: name,
        time: new Date(datetime)
    };

    events.push(event);

    sortEvents();
    renderList();

    document.getElementById("eventName").value = "";
}

function deleteEvent() {
    const list = document.getElementById("eventList");
    const index = list.selectedIndex;

    if (index >= 0) {
        events.splice(index, 1);
        renderList();
    }
}

function sortEvents() {
    events.sort((a, b) => a.time - b.time);
}

function renderList() {
    const list = document.getElementById("eventList");
    list.innerHTML = "";

    events.forEach(event => {
        const option = document.createElement("option");
        option.text = `${event.time.toLocaleString()} - ${event.name}`;
        list.add(option);
    });
}

setInterval(() => {
    const now = new Date();
    events = events.filter(event => event.time > now);
    renderList();
}, 60000); 