/*****************BRGER MENI*********************/
document.getElementById("burger").onclick = function(){
    let menu = document.getElementById("burger_menu")
    menu.classList.toggle("show");
}



/********DIVS**********/

document.getElementById("notificationsCard")
    .addEventListener("click", function() {
        window.location.href = "obavjestenja.html";
    });

document.getElementById("scheduleCard")
    .addEventListener("click", function() {
        window.location.href = "raspored.html";
    });

document.getElementById("dutyCard")
    .addEventListener("click", function() {
        window.location.href = "dezurstvo.html";
    });

document.getElementById("testBellCard")
    .addEventListener("click", function() {
        window.location.href = "test.html";
    });
