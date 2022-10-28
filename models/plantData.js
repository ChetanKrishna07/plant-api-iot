const mongoose = require('mongoose');

const plantDataSchema = new mongoose.Schema({
    moisture: String,
    waterLevel: Number,
    postTime: {
        type: Date,
        immutable: true,
        default: () => Date.now(),
    },
})

module.exports = mongoose.model('plantData', plantDataSchema)