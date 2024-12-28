// Import the functions you need from the SDKs you need
import { initializeApp } from "https://www.gstatic.com/firebasejs/11.1.0/firebase-app.js";
import { getAnalytics } from "https://www.gstatic.com/firebasejs/11.1.0/firebase-analytics.js";
import { getAuth, createUserWithEmailAndPassword } from "https://www.gstatic.com/firebasejs/11.1.0/firebase-auth.js";

// Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyBeoetlU6poGtdXfKcuXYInysdQhdIK8Vw",
  authDomain: "gasta-e5b66.firebaseapp.com",
  projectId: "gasta-e5b66",
  storageBucket: "gasta-e5b66.appspot.com", // Perbaikan URL
  messagingSenderId: "1015069930052",
  appId: "1:1015069930052:web:1c03a9d7788ae8c9724211",
  measurementId: "G-KQT7GMQR5W",
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const analytics = getAnalytics(app);
const auth = getAuth(app);

// Event Listener for form submission
const submit = document.getElementById("submit");
submit.addEventListener("click", function (event) {
  event.preventDefault();

  // Get input values
  const email = document.getElementById("email").value;
  const password = document.getElementById("pass").value;

  // Validate input
  if (!email || !password) {
    alert("Email and password must not be empty!");
    return;
  }

  // Create user with email and password
  createUserWithEmailAndPassword(auth, email, password)
    .then((userCredential) => {
      // User successfully created
      console.log("User created:", userCredential.user);
      alert("Account successfully created!");

    })
    .catch((error) => {
      // Handle errors
      console.error("Error creating user:", error);
      alert(`Error: ${error.message}`);
    });
});
 