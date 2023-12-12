const express = require('express');
const router = express.Router();
const viewtransactions = require('../models/viewtransactions_model');

router.post('/',
    function (request, response) {
        viewtransactions.getById(request.body, function (err, dbResult) {
            if (err) {
                response.json(err);
            } else {
                response.json(dbResult);
            }
        })
    });

module.exports = router;
