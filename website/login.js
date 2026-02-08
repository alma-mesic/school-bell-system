// Login sistem
const loginBtn = document.getElementById("loginBtn");
const usernameInput = document.getElementById("username");
const passwordInput = document.getElementById("password");
const errorMsg = document.getElementById("errorMsg");

// Button i ikona unutar password inputa
const togglePassBtn = document.getElementById("togglePass");
const eyeIcon = document.getElementById("eyeIcon");

// Fiksni admin
const ADMIN_USER = "admin";
const ADMIN_PASS = "1234";

// Toggle password visibility
togglePassBtn.addEventListener("click", () => {
    if(passwordInput.type === "password") {
        passwordInput.type = "text";
        eyeIcon.src = "hide.png"; // zatvoreno oko
    } else {
        passwordInput.type = "password";
        eyeIcon.src = "show.png"; // otvoreno oko
    }
});

// Login dugme
loginBtn.addEventListener("click", () => {
    const user = usernameInput.value;
    const pass = passwordInput.value;

    if(user === ADMIN_USER && pass === ADMIN_PASS) {
        window.location.href = "index.html"; // Preusmjeravanje na home
    } else {
        errorMsg.textContent = "Pogrešan username ili lozinka!";
        passwordInput.value = "";
        usernameInput.value = "";
    }
});
