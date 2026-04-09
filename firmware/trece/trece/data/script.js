/**GLOBAL***/
const ESP_IP = "http://10.132.0.148";
//const ESP_IP = "http://10.90.198.148";
/*****************BRGER MENI*********************/
const burger = document.getElementById("burger");
if (burger) {
    burger.addEventListener("click", function (e) {
        e.stopPropagation();
        let menu = document.getElementById("burger_menu");
        menu.classList.toggle("show");
    });
}
document.addEventListener("click", function (e) {
    const menu = document.getElementById("burger_menu");
    const burgerBtn = document.getElementById("burger");

    if (!menu.classList.contains("show")) return;

    if (!menu.contains(e.target) && !burgerBtn.contains(e.target)) {
        menu.classList.remove("show");
    }
});

const menu = document.getElementById("burger_menu");
if (menu) {
    menu.addEventListener("click", function (e) {
        e.stopPropagation();
    });
}
/********DIVS - main**********/

const notificationsCard = document.getElementById("notificationsCard");
if (notificationsCard) {
    notificationsCard.addEventListener("click", function () {
        window.location.href = "obavjestenja.html";
    });
}

const scheduleCard = document.getElementById("scheduleCard");
if (scheduleCard) {
    scheduleCard.addEventListener("click", function () {
        window.location.href = "raspored.html";
    });
}

const dutyCard = document.getElementById("dutyCard");
if (dutyCard) {
    dutyCard.addEventListener("click", function () {
        window.location.href = "dezurstvo.html";
    });
}

const testBellCard = document.getElementById("testBellCard");
if (testBellCard) {
    testBellCard.addEventListener("click", function () {
        window.location.href = "test.html";
    });
}


/*****************RASPORED*********************/

const redovni = [
    {
        cas: 1,
        pocetak: "7:30",
        kraj: "8:15"
    },
    {
        cas: 2,
        pocetak: "8:20",
        kraj: "9:05"
    },
    {
        cas: 3,
        pocetak: "9:10",
        kraj: "9:55"
    },
    {
        cas: 4,
        pocetak: "10:15",
        kraj: "11:00"
    },
    {
        cas: 5,
        pocetak: "11:05",
        kraj: "11:50"
    },
    {
        cas: 6,
        pocetak: "11:55",
        kraj: "12:40"
    },
    {
        cas: 7,
        pocetak: "12:45",
        kraj: "13:30"
    },
    {
        cas: 8,
        pocetak: "13:35",
        kraj: "14:20"
    },
    {
        cas: 9,
        pocetak: "14:25",
        kraj: "15:10"
    },
    {
        cas: 10,
        pocetak: "15:30",
        kraj: "16:15"
    },
    {
        cas: 11,
        pocetak: "16:20",
        kraj: "17:05"
    },
    {
        cas: 12,
        pocetak: "17:10",
        kraj: "17:55"
    },
    {
        cas: 13,
        pocetak: "18:00",
        kraj: "18:45"
    }
]

const skraceni = [
    {
        cas: 1,
        pocetak: "7:30",
        kraj: "8:00"
    },
    {
        cas: 2,
        pocetak: "8:05",
        kraj: "8:35"
    },
    {
        cas: 3,
        pocetak: "8:40",
        kraj: "9:10"
    },
    {
        cas: 4,
        pocetak: "9:30",
        kraj: "10:00"
    },
    {
        cas: 5,
        pocetak: "10:05",
        kraj: "10:35"
    },
    {
        cas: 6,
        pocetak: "10:40",
        kraj: "11:10"
    },
    {
        cas: 7,
        pocetak: "11:15",
        kraj: "11:45"
    },
    {
        cas: 8,
        pocetak: "11:50",
        kraj: "12:20"
    },
    {
        cas: 9,
        pocetak: "12:25",
        kraj: "12:55"
    },
    {
        cas: 10,
        pocetak: "13:15",
        kraj: "13:45"
    },
    {
        cas: 11,
        pocetak: "13:50",
        kraj: "14:20"
    },
    {
        cas: 12,
        pocetak: "14:25",
        kraj: "14:55"
    },
    {
        cas: 13,
        pocetak: "15:00",
        kraj: "15:30"
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

        red.id = `${tabelaId === "redovno" ? "r" : "s"}-cas-${casObj.cas}`;

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

document.addEventListener("DOMContentLoaded", function () {
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

/***************** POMOĆNE FUNKCIJE ZA SLANJE *****************/

async function posaljiZvono(status) {

    try {

        await fetch(`${ESP_IP}/api/data`, {

            method: 'POST',

            headers: { 'Content-Type': 'application/json' },

            body: JSON.stringify({

                tip: "zvono",

                akcija: status ? "start" : "stop"

            })

        });

    } catch (e) { console.log("ESP offline"); }

}



async function posaljiEmergency(aktivno) {
    try {
        await fetch(`${ESP_IP}/api/data`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                tip: "emergency",
                stanje: aktivno
            })

        });

    } catch (e) { console.log("ESP offline"); }
}



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
        posaljiZvono(aktivno);

        //pokreniSveZaTest();

    });

    updateUI();
});

/*****************EMERGENCY*********************/
let emergencyActive = false;

const emergencyBtn = document.querySelector(".emergency-btn");
const burgerBtn = document.getElementById("burger");

if (emergencyBtn) {
    emergencyBtn.onclick = async function (e) {
        if (!emergencyActive) {
            const potvrda = confirm("Da li ste sigurni da želite aktivirati HITNO ZVONO?");
            if (!potvrda) return;

            emergencyActive = true;
            activateEmergency();
            await posaljiEmergency(true);
        }else {
            const potvrdaStop = confirm("Da li želite zaustaviti emergency stanje?");
            if (!potvrdaStop) return;

            emergencyActive = false;
            deactivateEmergency();
            await posaljiEmergency(false);
        }
    };
}
function activateEmergency() {
    const sviElementi = document.querySelectorAll("button, a, input");
    sviElementi.forEach(el => {
        if (!el.classList.contains("emergency-btn")) {
            el.disabled = true;
            el.style.opacity = "1";
            el.style.pointerEvents = "none"
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



/******************* OBAVJEŠTENJA (slanje) *******************/
setInterval(() => {
    const now = new Date();
    events = events.filter(event => event.time > now);
    renderList();
}, 60000);

let listaObavjestenja = JSON.parse(localStorage.getItem("sacuvanaObavjestenja")) || [];

// Pretvara stringove iz localStorage natrag u Date objekte
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

    azurirajSve();
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
    azurirajSve();
    posaljiNaESP(true);
}

function azurirajSve() {
    localStorage.setItem("sacuvanaObavjestenja", JSON.stringify(listaObavjestenja));
    renderList();
}

//SLANJE NA ESP (DANAS I SUTRA)
async function posaljiNaESP(tihoSlanje = false) {

    if (listaObavjestenja.length === 0 && !tihoSlanje) {
        alert("Lista obavještenja je prazna!");
        return;
    }

    const sada = new Date();
    const krajSutra = new Date();
    krajSutra.setDate(krajSutra.getDate() + 1);
    krajSutra.setHours(23, 59, 59, 999);

    const zaMatricu = listaObavjestenja.filter(e => {
        const t = new Date(e.time);
        return t >= sada && t <= krajSutra;
    });

    const podaciZaSlanje = {
        tip: "obavijesti", 
        lista: zaMatricu.map(e => {
            const d = e.time;
            return{
                naziv: e.name,
                datumVrijeme: `${d.getFullYear()}-${String(d.getMonth() + 1).padStart(2, '0')}-${String(d.getDate()).padStart(2, '0')} ${String(d.getHours()).padStart(2, '0')}:${String(d.getMinutes()).padStart(2, '0')}`
            };
        })
    };

    try {
        const response = await fetch(`${ESP_IP}/api/data`, { // Putanja mora biti ista kao za raspored
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(podaciZaSlanje)
        });

        if (response.ok && !tihoSlanje) {
            //alert("Podaci na matrici ažurirani!");
            showPopup("Podaci na matrici ažurirani!", true);
        }
        
    } catch (error) {
        alert("ESP32 nedostupan.");
        //showPopup("ESP32 nedostupan!", false);
    }
}

document.addEventListener("DOMContentLoaded", () => {
    renderList();
    const sendBtn = document.querySelector(".sendBtn");
    if (sendBtn) sendBtn.onclick = () => posaljiNaESP();
});

// Automatsko čišćenje starih obavještenja svakih 10 sekundi
setInterval(() => {
    const now = new Date();
    const prethodnaDuzina = listaObavjestenja.length;

    // Briše samo one čije je vrijeme prošlo
    listaObavjestenja = listaObavjestenja.filter(e => new Date(e.time) > now);

    if (listaObavjestenja.length !== prethodnaDuzina) {
        azurirajSve();
        posaljiNaESP(true);
    }
}, 10000);


/***********************DEŽURSTVO************************ */
const profesori = [
    "Asmir Mandžukić", "Edin Zulfić", "Slađana Pejić", "Melisa Zahirović",
    "Damir Kunosić", "Jasmina Omerdić", "Naza Dadanović", "Medina Bristić",
    "Radislav Stojak", "Samir Tokić", "Muamer Halilović", "Muamer Jusić"
];

const dani = ["Ponedjeljak", "Utorak", "Srijeda", "Četvrtak", "Petak"];
const sati = [
    "07:00 do 08:00", "08:00 do 09:00", "09:00 do 10:00", "10:00 do 11:00",
    "11:00 do 12:00", "12:00 do 13:00", "13:00 do 14:00", "14:00 do 15:00",
    "15:00 do 16:00", "16:00 do 17:00", "17:00 do 18:00", "18:00 do 19:00"
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

    inp.addEventListener("input", function () {
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

    inp.addEventListener("keydown", function (e) {
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

/*document.getElementById("ucitaj-json").addEventListener("click", () => {
    const inputFile = document.createElement("input");
    inputFile.type = "file";
    inputFile.accept = ".json";

    inputFile.addEventListener("change", (event) => {
        const file = event.target.files[0];
        if (!file) return;

        const reader = new FileReader();
        reader.onload = (e) => {
            try {
                const data = JSON.parse(e.target.result);
                popuniTabelu(data);
                // Sačuvaj odmah u localStorage da ostane i nakon zatvaranja
                localStorage.setItem("dezurniData", JSON.stringify(data));
                //alert("JSON učitan i raspored popunjen!");
                showPopup("JSON učitan i raspored popunjen!", true);                
            } catch (err) {
                //alert("Greška prilikom učitavanja JSON-a: " + err.message);
                showPopup("Greška prilikom učitavanja JSON-a: " + err.message, false);
            }
        };
        reader.readAsText(file);
    });

    inputFile.click();
});*/

function exporting() {
    const headerRow = ["Dan", "Datum", ...sati]; 
    const dataForExcel = [headerRow];

    for (let i = 1; i < table.rows.length; i++) {
        const row = table.rows[i];
        const dan = row.cells[0].innerText;
        const datum = row.cells[1].querySelector("input")?.value || "";
        const currentRow = [dan, datum];
        
        for (let j = 2; j < row.cells.length; j++) {
            const prof = row.cells[j].querySelector("input")?.value || "";
            currentRow.push(prof);
        }
        dataForExcel.push(currentRow);
    }
    const worksheet = XLSX.utils.aoa_to_sheet(dataForExcel);
    const workbook = XLSX.utils.book_new();
    XLSX.utils.book_append_sheet(workbook, worksheet, "Dežurstvo");
    XLSX.writeFile(workbook, "Raspored_Dezurstva_ETS.xlsx");
}

document.getElementById("exporting").addEventListener("click", exporting);

function popuniTabelu(data) {
    for (let i = 1; i < table.rows.length; i++) {
        const row = table.rows[i];
        const dan = row.cells[0].innerText;
        if (!data[dan]) continue;

        const dateInput = row.cells[1].querySelector("input");
        dateInput.value = data[dan].date || "";

        for (let j = 2; j < row.cells.length; j++) {
            const timeSlot = table.rows[0].cells[j].innerText;
            const profInput = row.cells[j].querySelector("input");
            profInput.value = data[dan].schedule[timeSlot] || "";
        }
    }
}

window.addEventListener("DOMContentLoaded", () => {
    const savedData = localStorage.getItem("dezurniData");
    if (savedData) {
        popuniTabelu(JSON.parse(savedData));
    }
});

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
    //alert("Raspored sačuvan!");
    showPopup("Raspored sačuvan!", true);
    console.log(data);
});

document.getElementById("posalji-dezurne").addEventListener("click", () => {
    const matrix = [];
    for (let i = 1; i < table.rows.length; i++) {
        const row = table.rows[i];
        const dayArray = [];
        for (let j = 2; j < row.cells.length; j++) {
            let val = row.cells[j].querySelector("input")?.value.trim();
            if (!val) val = "nema profesora";
            dayArray.push(val);
        }
        matrix.push(dayArray);
    }
    const payload = {
        tip: "dezurstvo",
        data: matrix
    };
    console.log(payload);
    fetch("/api/data", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify(payload)
    })
    .then(r => r.json())
    .then(d => showPopup("Poslano na ESP",true))
    .catch(e => console.error(e));

});
/*******************RASPORED (slanje)********************/
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
//prikaziRasporede();
setInterval(osvjeziStatus, 1000); // Radi svake sekunde kao Timer u C#

function posaljiRasporedNaESP() {
    const tipRasporeda = document.querySelector('input[name="raspored"]:checked').value;
    const podaciZaESP = tipRasporeda === "redovno" ? redovni : skraceni;

    const paket = {
        tip: "raspored", // Mora biti "raspored" da bi handleJson ušao u pravi if
        casovi: podaciZaESP.map(c => ({
            dan: "Svi", // Možeš dodati logiku za dan ako treba
            cas: c.cas,
            pocetak: c.pocetak,
            kraj: c.kraj,
            dezurni: ""
        }))
    };

    fetch(`${ESP_IP}/api/data`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(paket)
    })
        .then(() => showPopup("Raspored poslat na ESP32!",true));
}

/**********************ZVONO test (slanje)************************/
let testAktivno = false;
function pokreniSveZaTest() {
    let d = document.getElementById("test_zvona");
    testAktivno = !testAktivno;

    const paket = {
        tip: "zvono",
        akcija: testAktivno ? "start" : "stop"
    };

    fetch(`${ESP_IP}/api/data`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(paket)
    });

    d.textContent = testAktivno ? "Zaustavi" : "Pokreni";
}


/**********************SYSTEM (slanje)**********************/
// script.js
let ledUpaljen = true; // globalna varijabla

document.addEventListener("DOMContentLoaded", () => {
    const dugme = document.getElementById("led_btn");
    if (!dugme) return;

    dugme.addEventListener("click", () => {
        upravljajLedTrakom(dugme); 
    });
});

function upravljajLedTrakom(dugme) {
    ledUpaljen = !ledUpaljen;

    const paket = {
        naredba: "LED_CONTROL",
        stanje: ledUpaljen ? "ON" : "OFF"
    };

    console.log("Šaljem paket:", paket);

    fetch(`${ESP_IP}/api/settings`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(paket)
    })
    .then((res) => {
        console.log("Response status:", res.status);
        
        const stilovi = document.getElementById("stilovi");
        const colorBox = document.getElementById("letter_color_change");
        const saveBtn = document.getElementById("ponudjeni-stil");

        if (ledUpaljen) {
            dugme.textContent = "Ugasi LED traku";
            if (colorBox) colorBox.disabled = false;
            if (saveBtn) saveBtn.disabled = false;
            showPopup("LED traka je uključena. Sistem aktivan.",true);
        } else {
            dugme.textContent = "Upali LED traku";
            if (stilovi) stilovi.style.pointerEvents = "auto";
            if (colorBox) colorBox.disabled = true;
            if (saveBtn) saveBtn.disabled = true;
            showPopup("LED traka je ugašena. Sistem neaktivan.",false);
        }
    })
    .catch(err => showPopup("ESP32 nedostupan za LED kontrolu.",false));
}

async function posaljiBoju(tip, elementId) {
    const colorInput = document.getElementById(elementId);
    if (!colorInput) return;

    // uzmi hex kod boje i pretvori u RGB
    const hex = colorInput.value;
    const r = parseInt(hex.substring(1, 3), 16);
    const g = parseInt(hex.substring(3, 5), 16);
    const b = parseInt(hex.substring(5, 7), 16);

    try {
        await fetch(`${ESP_IP}/api/settings`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                naredba: "SET_COLOR",
                tip: tip,
                r: r,
                g: g,
                b: b
            })
        });
        showPopup(`Boja za "${tip}" je poslana: R=${r}, G=${g}, B=${b}`,true);
    } catch (e) {
        console.log("ESP offline ili greška u slanju boje");
        //alert("Neuspjelo slanje boje. Provjeri ESP32.");
        showPopup("Neuspjelo slanje boje. Provjeri ESP32.",false);
    }
}
/*********************** SETTINGS & BOJE ************************/

// 1. Funkcija za uživo ažuriranje kvadratića (već je imaš u kodu)
function setupColorPreview(inputId, previewId) {
    const colorInput = document.getElementById(inputId);
    const preview = document.getElementById(previewId);

    if (colorInput && preview) {
        preview.style.backgroundColor = colorInput.value;
        colorInput.addEventListener('input', () => {
            preview.style.backgroundColor = colorInput.value;
        });
    }
}

// 2. Pomoćna funkcija za pretvaranje HEX u RGB
function hexToRgb(hex) {
    const r = parseInt(hex.slice(1, 3), 16);
    const g = parseInt(hex.slice(3, 5), 16);
    const b = parseInt(hex.slice(5, 7), 16);
    return { r, g, b };
}

// 3. Glavna funkcija koja šalje JSON paket na ESP32
async function posaljiBoju(tip, inputId) {
    const colorInput = document.getElementById(inputId);
    if (!colorInput) return;

    const rgb = hexToRgb(colorInput.value);
    const paket = {
        naredba: "SET_COLOR",
        tip: tip,
        r: rgb.r,
        g: rgb.g,
        b: rgb.b
    };

    try {
        const response = await fetch(`${ESP_IP}/api/settings`, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(paket)
        });
        if (response.ok) {
            let naziv = "";
            if (tip === "boja_sata") {
                naziv = "Boja sata";
            }
            else if (tip === "boja_scroll") {
                naziv = "Boja obavijesti";
            }
            else {
                naziv = tip;
            }
            //alert(`${naziv} sačuvana!\nRGB: ${rgb.r}, ${rgb.g}, ${rgb.b}`);
            showPopup(`${naziv} sačuvana!\nRGB: ${rgb.r}, ${rgb.g}, ${rgb.b}`,true);
        }
    } catch (error) {
        //alert(`Greška pri slanju boje!\nRGB: ${rgb.r}, ${rgb.g}, ${rgb.b}`);
        showPopup(`Greška pri slanju boje!\nRGB: ${rgb.r}, ${rgb.g}, ${rgb.b}`,false);
    }
}
function updatePreview() {
    // 1. Uzmi vrijednosti iz select menija
    const letterFont = document.getElementById('select_letter_font').value;
    const clockFont = document.getElementById('select_clock_font').value;

    // 2. Pronađi elemente u panelu
    const clockElement = document.getElementById('preview_clock');
    const textElements = [document.getElementById('preview_text1'), document.getElementById('preview_text2')];

    // 3. Primijeni klase za Sat
    clockElement.className = ''; // Briše stare klase
    clockElement.classList.add(clockFont);

    // 4. Primijeni klase za Obavještenja
    textElements.forEach(el => {
        el.className = ''; 
        el.classList.add(letterFont);
    });
}

// Funkcija za slova
async function sacuvajFontSlova() {
    const letterFont = document.getElementById('select_letter_font').value;

    const paket = {
        naredba: "SET_FONT_LETTER",
        font_tekst: letterFont
    };

    try {
        const response = await fetch(`${ESP_IP}/api/settings`, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(paket)
        });

        if (response.ok) {
            showPopup("Font slova poslan: " + letterFont, true);
        } else {
            //alert("Greška prilikom slanja fonta slova",false);
            showPopup("Greška prilikom slanja fonta slova",false);
        }
    } catch (error) {
        alert("ESP32 nije dostupan");
    }
}

// Funkcija za sat
async function sacuvajFontSata() {
    const clockFont = document.getElementById('select_clock_font').value;

    const paket = {
        naredba: "SET_FONT_CLOCK",
        font_sat: clockFont
    };

    try {
        const response = await fetch(`${ESP_IP}/api/settings`, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(paket)
        });

        if (response.ok) {
            showPopup("Font sata poslan: " + clockFont,true);
        } else {
            showPopup("Greška prilikom slanja fonta sata",false);
        }
    } catch (error) {
        alert("ESP32 nije dostupan");
    }
}


async function ponudjeni_stil(){
    const selected = document.querySelector('input[name="tip"]:checked');
    if (!selected) {
        alert("Izaberi stil");
        return;
    }

    const paket = {
        naredba: "LED_MODE",
        mode: selected.value
    };

    try {
        const response = await fetch(`${ESP_IP}/api/settings`, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(paket)
        });

        if (response.ok) {
            showPopup("Poslano: " + paket,true);
        } else {
            showPopup("Greška prilikom slanja stil",false);
        }
    } catch (error) {
        alert("ESP32 nije dostupan");
    }
}

/*document.getElementById("ponudjeni-stil").addEventListener("click", async () => {
    if (!selected) {
        alert("Izaberi stil");
        return;
    }
    const paket = {
        naredba: "LED_MODE",
        mode: selected.value
    };
    /*try {
        await fetch(`${ESP_IP}/api/settings`, {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(json)
        });
        console.log("Poslano:", json);
    } catch (err) {
        console.error(err);
    }

    try {
        const response = await fetch(`${ESP_IP}/api/settings`, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(paket)
        });

        if (response.ok) {
            showPopup("Poslano: " + json,true);
        } else {
            showPopup("Greška prilikom slanja stil",false);
        }
    } catch (error) {
        alert("ESP32 nije dostupan");
    }
});*/


/********************** PROFIL & WIFI POSTAVKE **********************/
function cancel(type){
    if (type === "username") {
        document.getElementById("user-name-old").value = "";
        document.getElementById("user-name-new").value = "";
        document.getElementById("user-name-new-confirmation").value = "";
    }

    if (type === "password") {
        document.getElementById("password-old").value = "";
        document.getElementById("password-new").value = "";
        document.getElementById("password-new-confirmation").value = "";
    }
}

async function promijeniUsername() {
    console.log("Kliknuto promjeni username");
    const staro = document.getElementById("user-name-old").value;
    const novo = document.getElementById("user-name-new").value;
    const potvrda = document.getElementById("user-name-new-confirmation").value;

    if (novo !== potvrda) {
        alert("Novi username i potvrda se ne podudaraju!");
        return;
    }

    const paket = { 
        naredba: "UPDATE_USER", 
        staro: staro, 
        novo: novo 
    };
    posaljiNaProfilAPI(paket);
}


async function promijeniSifru() {
    console.log("Kliknuto promjeni sifru");
    const stara = document.getElementById("password-old").value;
    const nova = document.getElementById("password-new").value;
    const potvrda = document.getElementById("password-new-confirmation").value;

    if (nova !== potvrda) {
        alert("Nova šifra i potvrda se ne podudaraju!");
        return;
    }

    const paket = { 
        naredba: "UPDATE_PASS", 
        stara: stara, 
        nova: nova 
    };
    posaljiNaProfilAPI(paket);
}


/*async function sacuvajWifi() {
    console.log("Kliknuto Sačuvaj WiFi");
    const ssid = document.getElementById("wifi-name").value;
    const pass = document.getElementById("wifi-password").value;

    if (!ssid || !pass) {
        alert("Popunite oba WiFi polja!");
        return;
    }

    const paket = { naredba: "UPDATE_WIFI", ssid: ssid, pass: pass };
    posaljiNaProfilAPI(paket, "WiFi postavke sačuvane! ESP32 će se restartovati.");
}*/

// Pomoćna funkcija za slanje
async function posaljiNaProfilAPI(paket) {
    try {
        const response = await fetch(`${ESP_IP}/api/profile`, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(paket)
        });
        if (response.ok) showPopup("Promjene sačuvane!",true);
        else showPopup("Greška: Server nije prihvatio promjene.",false);
    } catch (error) {
        alert("Greška pri komunikaciji sa ESP32.");
    }
}

async function ucitajTrenutniUsername() {
    const userDisplay = document.getElementById("username");
    if (!userDisplay) return; // Ako nismo na profil.html, prekini

    try {
        const response = await fetch(`${ESP_IP}/api/get_user`);
        if (response.ok) {
            const ime = await response.text();
            userDisplay.textContent = ime; // Upisuje ime u <p id="username"></p>
        } else {
            userDisplay.textContent = "Gost";
        }
    } catch (error) {
        console.log("Greška pri učitavanju imena");
        userDisplay.textContent = "Offline";
    }
}

// Povezivanje dugmića sa funkcijama (Inicijalizacija)
document.addEventListener("DOMContentLoaded", function () {

    ucitajTrenutniUsername();

    const btnUser = document.getElementById("change-user");
    const btnPass = document.getElementById("Promijeni-user");

    ///if (btnUser) btnUser.onclick = promijeniUsername();
    //if (btnPass) btnPass.onclick = promijeniSifru();

    setupColorPreview('letter_color_change', 'letter_color_preview');
    setupColorPreview('marix_time_color_change', 'marix_time_color_preview');
    setupColorPreview('matrix_letter_color_change', 'matrix_letter_color_preview');

    // Osluškuj promjene na select menijima
    const selectLetter = document.getElementById('select_letter_font');
    const selectClock = document.getElementById('select_clock_font');

    if (selectLetter) {
        selectLetter.addEventListener('change', updatePreview);
    }
    if (selectClock) {
        selectClock.addEventListener('change', updatePreview);
    }
    updatePreview();

});

/** GLOBALNA FUNKCIJA ZA POPUP */
function showPopup(message, success = true) {
    // Provjeri postoji li već popup, ako ne kreiraj
    let popup = document.getElementById("globalPopup");
    if (!popup) {
        popup = document.createElement("div");
        popup.id = "globalPopup";
        popup.style.position = "fixed";
        popup.style.top = "20px";
        popup.style.right = "20px";
        popup.style.padding = "15px 25px";
        popup.style.borderRadius = "5px";
        popup.style.fontFamily = "Arial, sans-serif";
        popup.style.fontSize = "14px";
        popup.style.color = "#fff";
        popup.style.zIndex = 9999;
        popup.style.boxShadow = "0 2px 8px rgba(0,0,0,0.3)";
        popup.style.transition = "opacity 0.3s ease, transform 0.3s ease";
        document.body.appendChild(popup);
    }

    // Postavi boju i tekst
    popup.style.backgroundColor = success ? "#2ecc71" : "#E74C3C"; // zelena ili crvena
    popup.textContent = message;

    // Animacija prikaza
    popup.style.opacity = "1";
    popup.style.transform = "translateY(0px)";

    // Automatski sakrij nakon 3 sekunde
    setTimeout(() => {
        popup.style.opacity = "0";
        popup.style.transform = "translateY(-20px)";
    }, 3000);
}