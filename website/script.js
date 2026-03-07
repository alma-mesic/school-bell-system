/**GLOBAL***/
let events = [];
const ESP_IP = "http://10.132.0.148";
/*****************BRGER MENI*********************/
const burger = document.getElementById("burger");
if (burger) {
    burger.addEventListener("click", function(e){
    e.stopPropagation(); 
    let menu = document.getElementById("burger_menu");
    menu.classList.toggle("show");
});
}
document.addEventListener("click", function(e) {
    const menu = document.getElementById("burger_menu");
    const burgerBtn = document.getElementById("burger");

    if (!menu.classList.contains("show")) return;

    if (!menu.contains(e.target) && !burgerBtn.contains(e.target)) {
        menu.classList.remove("show");
    }
});

const menu = document.getElementById("burger_menu");
if (menu) {
    menu.addEventListener("click", function(e) {
        e.stopPropagation();
    });
}
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

    if (!tabela) return;
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

    // SIGURNOSNA PROVJERA: Radi samo ako oba elementa postoje na stranici
    if (colorInput && preview) {
        preview.style.backgroundColor = colorInput.value;

        colorInput.addEventListener('input', () => {
            preview.style.backgroundColor = colorInput.value;
        });
    }
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
    
    if (!list) return; 

    list.innerHTML = ""; 

    events.forEach(event => {
        const option = document.createElement("option");
        // Provjeravamo je li event.time već Date objekat ili ga treba pretvoriti
        const d = (event.time instanceof Date) ? event.time : new Date(event.time);
        option.text = `${d.toLocaleString()} - ${event.name}`;
        list.add(option);
    });
}

setInterval(() => {
    const now = new Date();
    events = events.filter(event => event.time > now);
    renderList();
}, 60000); 

/******************* OBAVJEŠTENJA (slanje) *******************/

let listaObavjestenja = JSON.parse(localStorage.getItem("sacuvanaObavjestenja")) || [];

listaObavjestenja = listaObavjestenja.map(e => ({
    name: e.name,
    time: new Date(e.time)
}));

function addEvent() {
    const name = document.getElementById("eventName").value;
    const datetime = document.getElementById("datetime").value;

    if (!name || !datetime) {
        alert("Popuni sva polja!");
        return;
    }

    const noviEvent = {
        name: name,
        time: new Date(datetime)
    };

    listaObavjestenja.push(noviEvent);
    // Sortiranje: najbliži događaji idu prvi
    listaObavjestenja.sort((a, b) => a.time - b.time);

    //  Da ostane tu i nakon osvježavanja stranice
    localStorage.setItem("sacuvanaObavjestenja", JSON.stringify(listaObavjestenja));

    renderList();
    document.getElementById("eventName").value = "";
}

function renderList() {
    const list = document.getElementById("eventList");
    if (!list) return;
    
    list.innerHTML = "";
    listaObavjestenja.forEach(event => {
        const option = document.createElement("option");
        // Formatiranje datuma 
        option.text = `${event.time.toLocaleString('bs-BA')} - ${event.name}`;
        list.add(option);
    });
}

function deleteEvent() {
    const list = document.getElementById("eventList");
    if (!list || list.selectedIndex < 0) return;
    
    listaObavjestenja.splice(list.selectedIndex, 1);
    localStorage.setItem("sacuvanaObavjestenja", JSON.stringify(listaObavjestenja));
    renderList();
}

/******************* SLANJE NA ESP (DANAS I SUTRA) *******************/

async function posaljiNaESP() {
    if (listaObavjestenja.length === 0) {
        alert("Lista obavještenja je prazna!");
        return;
    }

    const sada = new Date();
    const krajSutra = new Date();
    krajSutra.setDate(krajSutra.getDate() + 1);
    krajSutra.setHours(23, 59, 59, 999);

    // Filter: Šaljemo samo ono što je za danas i sutra
    const zaMatricu = listaObavjestenja.filter(e => {
        const t = new Date(e.time);
        return t >= sada && t <= krajSutra;
    });

    const podaciZaSlanje = {
        tip: "obavjestenje",
        vrijednost: zaMatricu.map(e => ({
            ime: e.name,
            vrijeme: Math.floor(e.time.getTime() / 1000)
        }))
    };

    // Alert da vidiš šta ide na sat prije slanja
    alert("PODACI SPREMNI:\nŠaljem " + zaMatricu.length + " obavještenja (danas/sutra).");

    try {
        const response = await fetch(`${ESP_IP}/poshalji`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(podaciZaSlanje)
        });

        if (response.ok) {
            alert("Uspješno poslano na ESP32!");
        } else {
            alert("Greška pri slanju na server.");
        }
    } catch (error) {
        alert("ESP32 nije dostupan, ali su obavještenja sačuvana u browseru.");
    }
}

// Inicijalizacija
document.addEventListener("DOMContentLoaded", function() {
    renderList();
    
    const sendBtn = document.querySelector(".sendBtn");
    if (sendBtn) {
        sendBtn.onclick = posaljiNaESP;
    }
});

// Automatsko čišćenje starih obavještenja svakih 10 sekundi
setInterval(() => {
    const now = new Date();
    const prethodnaDuzina = listaObavjestenja.length;
    
    // Briše samo one čije je vrijeme prošlo
    listaObavjestenja = listaObavjestenja.filter(e => new Date(e.time) > now);
    
    if (listaObavjestenja.length !== prethodnaDuzina) {
        localStorage.setItem("sacuvanaObavjestenja", JSON.stringify(listaObavjestenja));
        renderList();
    }
}, 10000);


/***********************DEŽURSTVO************************ */
const profesori = [
    "Asmir Mandžukić", "Edin Zulfić", "Slađana Pejić", "Melisa Zahirović",
    "Damir Kunosić", "Jasmina Omerdić", "Naza Dadanović", "Medina Bristić",
    "Radislav Stojak", "Samir Tokić", "Muamer Halilović", "Muamer Jusić"
];

const dani = ["Ponedjeljak","Utorak","Srijeda","Četvrtak","Petak"];
const sati = [
    "07:00 do 08:00","08:00 do 09:00","09:00 do 10:00","10:00 do 11:00",
    "11:00 do 12:00","12:00 do 13:00","13:00 do 14:00","14:00 do 15:00",
    "15:00 do 16:00","16:00 do 17:00","17:00 do 18:00","18:00 do 19:00"
];

const dezProfa = document.getElementById("dez-profa");
const table = document.createElement("table");
dezProfa.appendChild(table);

const header = table.insertRow();
header.insertCell().outerHTML = "<th>Dan</th>";
header.insertCell().outerHTML = "<th>Datum</th>";
sati.forEach(sat => {
    const th = document.createElement("th");
    th.textContent = sat;
    header.appendChild(th);
});

dani.forEach(dan => {
    const row = table.insertRow();
    row.insertCell().textContent = dan;
    const dateCell = row.insertCell();
    const dateInput = document.createElement("input");
    dateInput.type = "date";
    dateCell.appendChild(dateInput);

    sati.forEach(() => {
        const cell = row.insertCell();
        const input = document.createElement("input");
        input.type = "text";
        input.placeholder = "Odaberi profesora";
        cell.appendChild(input);
        autocomplete(input, profesori); 
    });
});

function autocomplete(inp, arr) {
    let currentFocus;

    inp.addEventListener("input", function() {
        closeAllLists();
        if (!this.value) return false;
        currentFocus = -1;

        const list = document.createElement("div");
        list.setAttribute("class", "autocomplete-items");
        this.parentNode.appendChild(list);

        arr.forEach(item => {
            if (item.toLowerCase().includes(this.value.toLowerCase())) {
                const itemDiv = document.createElement("div");
                itemDiv.innerHTML = "<strong>" + item.substr(0, this.value.length) + "</strong>";
                itemDiv.innerHTML += item.substr(this.value.length);
                itemDiv.addEventListener("click", () => {
                    inp.value = item;
                    closeAllLists();
                });
                list.appendChild(itemDiv);
            }
        });
    });

    inp.addEventListener("keydown", function(e) {
        const items = this.parentNode.querySelectorAll(".autocomplete-items div");
        if (!items) return;

        if (e.key === "ArrowDown") {
            currentFocus++;
            addActive(items);
        } else if (e.key === "ArrowUp") {
            currentFocus--;
            addActive(items);
        } else if (e.key === "Enter") {
            e.preventDefault();
            if (currentFocus > -1) items[currentFocus].click();
        }
    });

    function addActive(items) {
        if (!items) return false;
        removeActive(items);
        if (currentFocus >= items.length) currentFocus = 0;
        if (currentFocus < 0) currentFocus = items.length - 1;
        items[currentFocus].classList.add("autocomplete-active");
    }

    function removeActive(items) {
        items.forEach(item => item.classList.remove("autocomplete-active"));
    }

    function closeAllLists(elmnt) {
        const items = document.querySelectorAll(".autocomplete-items");
        items.forEach(item => {
            if (elmnt != item && elmnt != inp) item.parentNode.removeChild(item);
        });
    }

    document.addEventListener("click", (e) => closeAllLists(e.target));
}

document.getElementById("save-prof").addEventListener("click", () => {
    const data = {};
    for (let i = 1; i < table.rows.length; i++) {
        const row = table.rows[i];
        const dan = row.cells[0].innerText;
        const datum = row.cells[1].querySelector("input")?.value || "";
        data[dan] = { date: datum, schedule: {} };
        for (let j = 2; j < row.cells.length; j++) {
            const timeSlot = table.rows[0].cells[j].innerText;
            const prof = row.cells[j].querySelector("input")?.value || "";
            data[dan].schedule[timeSlot] = prof;
        }
    }
    localStorage.setItem("dezurniData", JSON.stringify(data));
    alert("Raspored sačuvan!");
    console.log(data);
});
/*******************RASPORED (slanje)********************/
function prikaziRasporede() {
    const tRedovno = document.querySelector("#redovno");
    const tSkraceno = document.querySelector("#skraceno");

    let header = `<thead><tr><th>Čas</th><th>Početak</th><th>Kraj</th></tr></thead><tbody>`;

    // Popuni redovnu tabelu
    tRedovno.innerHTML = header + redovni.map(c => 
        `<tr id="r-cas-${c.cas}"><td>${c.cas}.</td><td>${c.pocetak}</td><td>${c.kraj}</td></tr>`
    ).join("") + "</tbody>";

    // Popuni skraćenu tabelu
    tSkraceno.innerHTML = header + skraceni.map(c => 
        `<tr id="s-cas-${c.cas}"><td>${c.cas}.</td><td>${c.pocetak}</td><td>${c.kraj}</td></tr>`
    ).join("") + "</tbody>";
}

function osvjeziStatus() {
    // 1. Provjeri koji je raspored odabran na dugmićima
    const odabraniTip = document.querySelector('input[name="raspored"]:checked').value;
    const trenutniNiz = odabraniTip === "redovno" ? redovni : skraceni;
    const prefix = odabraniTip === "redovno" ? "r" : "s";

    // 2. Dobij trenutno vrijeme u minutama (npr. 07:45 -> 465 min)
    const sad = new Date();
    const trenutnoMin = sad.getHours() * 60 + sad.getMinutes();

    // Resetuj sve boje u tabelama
    document.querySelectorAll("tr").forEach(tr => tr.classList.remove("active-row"));

    let ispisZaMatricu = "Trenutno nema nastave";

    // 3. Prođi kroz časove i nađi gdje smo sad
    for (let c of trenutniNiz) {
        const [hP, mP] = c.pocetak.split(":").map(Number);
        const [hK, mK] = c.kraj.split(":").map(Number);
        
        const pocetakMin = hP * 60 + mP;
        const krajMin = hK * 60 + mK;

        if (trenutnoMin >= pocetakMin && trenutnoMin < krajMin) {
            const ostalo = krajMin - trenutnoMin;
            ispisZaMatricu = `Cas: ${c.cas} | Kraj: ${c.kraj} | Jos: ${ostalo}min`;
            
            // Oboji red u tabeli (zeleno)
            document.getElementById(`${prefix}-cas-${c.cas}`).classList.add("active-row");
            break;
        }
    }
    
    // Ovdje ispisujemo u konzolu, a sutra ćemo ovo fetch-ati na ESP32
    console.log(ispisZaMatricu);
    return ispisZaMatricu;
}

// Pokretanje svega
prikaziRasporede();
setInterval(osvjeziStatus, 1000); // Radi svake sekunde kao Timer u C#

function posaljiRasporedNaESP() {
    // 1. Uzmi trenutni ispis koji je funkcija osvjeziStatus izračunala
    // Da bismo to uradili lakše, izmijenićemo osvjeziStatus da vraća taj tekst
    const tekstZaMatricu = osvjeziStatus(); 

    // 2. Uzmi koji je tip rasporeda (da ESP zna cijelu tabelu)
    const tip = document.querySelector('input[name="raspored"]:checked').value;
    const podaciZaESP = tip === "redovno" ? redovni : skraceni;

    // 3. Spakuj u JSON
    const paket = {
        naredba: "azuriraj_raspored",
        tip: tip,
        ispis: tekstZaMatricu,
        svi_casovi: podaciZaESP
    };

    console.log("Šaljem JSON na ESP:", paket);

    // 4. Fetch poziv 
    fetch(`{ESP_IP}/api/raspored`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(paket)
    }).then(() => alert("Poslato na matricu!"));
}

/**********************ZVONO test (slanje)************************/
let testAktivno = false;
function pokreniSveZaTest() {
    let d = document.getElementById("test_zvona"); // Nađemo dugme
    testAktivno = !testAktivno; // Promijenimo stanje

    if (testAktivno) {
        d.textContent = "Zaustavi"; // Promijeni tekst na ekranu
        fetch(`{ESP_IP}/api/test`, { method: "POST", body: JSON.stringify({ naredba: "TEST_START" }) });
    } else {
        d.textContent = "Pokreni"; // Vrati tekst na ekranu
        fetch(`{ESP_IP}/api/test`, { method: "POST", body: JSON.stringify({ naredba: "TEST_STOP" }) });
    }
}

let emergencyAktivno = false;
function pokreniSveZaEmergency() {
    let d = document.querySelector(".emergency-btn");
    emergencyAktivno = !emergencyAktivno;

    if (emergencyAktivno) {
        d.textContent = "STOP EMERGENCY";
        fetch(`{ESP_IP}/api/emergency`, { method: "POST", body: JSON.stringify({ naredba: "EMERGENCY_START" }) });
    } else {
        d.textContent = "Emergency";
        fetch(`{ESP_IP}/api/emergency`, { method: "POST", body: JSON.stringify({ naredba: "EMERGENCY_STOP" }) });
    }
}

/**********************SYSTEM (slanje)**********************/
// --- FUNKCIJA ZA BRISANJE EEPROM-A ---
function obrisiSvePodatke() {
    if (confirm("Jeste li sigurni da želite obrisati SVE podatke (raspored, dežurstva, poruke)? Ova radnja je nepovratna!")) {
        fetch(`{ESP_IP}/api/settings`, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ naredba: "CLEAR_EEPROM" })
        })
        .then(() => alert("Komanda za brisanje poslata. Sistem će se restartovati."))
        .catch(err => alert("Greška: ESP32 nije dostupan."));
    }
}

// --- FUNKCIJA ZA LED TRAKU ---
let ledUpaljen = true; // Pretpostavljamo da je na početku upaljena
function upravljajLedTrakom(dugme) {
    ledUpaljen = !ledUpaljen;

    const paket = {
        naredba: "LED_CONTROL",
        stanje: ledUpaljen ? "ON" : "OFF"
    };

    fetch("{ESP_IP}/api/settings", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(paket)
    })
    .then(() => {
        if (ledUpaljen) {
            dugme.textContent = "Ugasi LED traku";
            dugme.style.backgroundColor = ""; // Vraća originalnu boju
        } else {
            dugme.textContent = "Upali LED traku";
            dugme.style.backgroundColor = "#2c3e50"; // Tamnija boja kad je ugašeno
        }
    })
    .catch(err => console.log("Greška pri kontroli LED trake."));
}