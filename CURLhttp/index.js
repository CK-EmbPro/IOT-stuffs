const express = require('express')
const multer = require('multer');
const app = express()

const storage = multer.diskStorage({
    destination: (req, file, cb)=>{
        cb(null, './CURLuploads')
    },

    filename: (req, file, cb)=>{
        cb(null, file.originalname)
    }
})
// Configuring destination in diskstorage and what will be stored there

const upload = multer({storage: storage})
// This is the creation of multer middleware defining the storage like destination and what will be stored there.

app.post('/upload',upload.single("file"), (req, res)=>{
    console.log("uploadded")
    res.send("received")
} )

app.get('/',(req, res)=>{
    res.send("Localhost access now")
    console.log("localhost access now")
})

app.listen(80, ()=>{
    console.log("listenining on port 3000");
})