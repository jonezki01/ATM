const express = require('express');
const router = express.Router();
const saldo = require('../models/saldo_model');

router.post('/',
    function (request, response) {
        saldo.post(request.body, function (err, dbResult) {
            if (err) {
                response.json(err);
            } else {
                console.log(dbResult);
                response.json(dbResult);
            }
        })
    });

module.exports = router;