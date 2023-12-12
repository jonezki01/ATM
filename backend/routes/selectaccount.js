const express = require('express');
const router = express.Router();
const withdraw = require('../models/selectaccount_model');

router.post('/',
    function (request, response) {
        withdraw.post(request.body, function (err, dbResult) {
            if (err) {
                response.json(err);
            } else {
                console.log(dbResult);
                response.json(dbResult);
            }
        })
    });

module.exports = router;