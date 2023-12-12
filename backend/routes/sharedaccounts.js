const express = require('express');
const router = express.Router();
const getsharedaccounts = require('../models/sharedaccounts_model');

router.post('/',
    function (request, response) {
        getsharedaccounts.post(request.body, function (err, dbResult) {
            if (err) {
                response.json(err);
            } else {
                console.log(dbResult);
                response.json(dbResult);
            }
        })
    });

module.exports = router;