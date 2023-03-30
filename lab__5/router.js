var express = require("express");
var router = express.Router();

const  credential = {
    email : "admin@gmail.com",
    password : "admin123"
}

// login user
router.post('/login', (req, res)=>{
    if(req.body.email == credential.email && req.body.password == credential.password){
        req.session.user = req.body.email;
        res.redirect('/route/dashboard');
    }else{
        res.render('base', { title: "Express", logout : "Incorrect login or password"})
    }
});

// route for dashboard
router.get('/footer', (req, res) => {
    if(req.session.user){
        res.render('footer', {user : req.session.user})
    }else{
        res.send("Unauthorize User")
    }
})

router.get('/goods', (req, res) => {
    if(req.session.user){
        res.render('goods', {user : req.session.user})
    }else{
        res.send("Unauthorize User")
    }
})

// route for dashboard
router.get('/dashboard', (req, res) => {
    if(req.session.user){
        res.render('dashboard', {user : req.session.user})
    }else{
        res.send("Unauthorize User")
    }
})



module.exports = router;