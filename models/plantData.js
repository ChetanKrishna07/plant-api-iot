const mongoose = require('mongoose');

const plantDataSchema = new mongoose.Schema({
    moisture: String,
    waterLevel: String,
    postTime: {
        type: Date,
        immutable: true,
        default: () => Date.now(),
    },
})

module.exports = mongoose.model('plantData', plantDataSchema)