const express = require('express');
const app = express();

app.set('view engine', 'ejs');

app.get('/', (req, res) => {
    const code = req.query.code;
    res.render('index', {code: code});
})

app.listen(4500, () => {
    
})