const mongoose = require('mongoose')
const cors = require('cors')
const bodyParser = require('body-parser')
const express = require('express')
const PlantData = require('./models/plantData')
const plantData = require('./models/plantData')

const app = express()
const port = process.env.PORT || 2000;

app.use(cors())
app.use(bodyParser.json())
app.use(bodyParser({urlencoded:true}))

mongoose.connect(
    'mongodb+srv://chetan:chetan@cluster0.xgz0mib.mongodb.net/?retryWrites=true&w=majority',
    (err) => {
        if(err) {
            console.log('err');
        } else {
            console.log('connected to db');
        }
    }
)

app.post('/addData', async(req, res) => {
    console.log('POST request at /addData');
    const plantDataObj = {
        moisture: req.body.moisture,
        waterLevel: req.body.waterLevel,
    }

    await PlantData.create(plantDataObj, (err, item) => {
        if (err) {
            console.log(err);
        } else {
            res.status(200).send("Successful")
        }
    })
})

app.get('/getData', async(req, res) => {
    console.log('GET request at /getData');
    try {
        let data = await PlantData.findOne().sort({postTime: -1})
        res.send(data);
    } catch(err) {
        console.log(err);
        res.send('Not Found');
    }
})

app.listen(port, () => {
    console.log(`Server running in localhost:${port}`);
})