const express = require('express');
const app = express();
const path = require('path');

const admin = require("firebase-admin");
const credentials = require("./serviceAccountKey.json");
 

admin.initializeApp({
    credential: admin.credential.cert(credentials)
});

app.use(express.json()); 
app.use(express.urlencoded({ extended: true}));


app.post('/login', async (req, res) => {
    try {
        console.log(req.body);
        const user = {
            email: req.body.email,
            password: req.body.password,
        };
        const userResponse = await admin.auth().createUser({
            email: user.email,
            password: user.password,
            emailVerified: false,
            disabled: false,
        });
        res.json(userResponse);
    } catch (error) {
        console.error('Error creating user:', error.message);
        res.status(400).json({ error: error.message });
    }
});

// Rute untuk menampilkan halaman login
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'Login Page.html'));
});

// Menyajikan file statis dari folder 'public'
app.use(express.static(path.join(__dirname, 'public')));


app.listen(3000, () => {
  console.log('Server berjalan di http://localhost:3000');
});
