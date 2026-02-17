/************************LOGIN************************/ 
 const loginBtn = document.getElementById("loginBtn");
const usernameInput = document.getElementById("username");
const passwordInput = document.getElementById("password");
const togglePassword = document.getElementById("togglePassword");
const errorMsg = document.getElementById("errorMsg");

const ADMIN_USER = "admin";
const ADMIN_PASS = "1234";

// Toggle password visibility
togglePassword.addEventListener("click", () => {
    const type = passwordInput.type === "password" ? "text" : "password";
    passwordInput.type = type;
    togglePassword.textContent = type === "password" ? "\u{1F441}" : "\u{1F441}\u{FE0E}"; // oko
});

// Login button
loginBtn.addEventListener("click", () => {
    const user = usernameInput.value.trim();
    const pass = passwordInput.value;

    if (user === ADMIN_USER && pass === ADMIN_PASS) {
        window.location.href = "index.html"; // Preusmjeravanje
    } else {
        errorMsg.textContent = "Pogrešan username ili lozinka!";
        usernameInput.value = "";
        passwordInput.value = "";
    }
});