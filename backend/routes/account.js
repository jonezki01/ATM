const express = require('express');
const router = express.Router();
const account = require('../models/account_model');

router.get('/',
    function (request, response) {
        account.getAll(function (err, dbResult) {
            if (err) {
                response.json(err);
            } else {
                console.log(dbResult);
                response.json(dbResult);
            }
        })
    });


router.get('/:id',
    function (request, response) {
        account.getById(request.params.id, function (err, dbResult) {
            if (err) {
                response.json(err);
            } else {
                response.json(dbResult);
            }
        })
    });


router.post('/', 
function(request, response) {
  account.add(request.body, function(err, dbResult) {
    if (err) {
      response.json(err);
      console.log(dbResult);
    } else {
      response.json(request.body);
      console.log(dbResult);
    }
  });
});


router.delete('/:id', 
function(request, response) {
  account.delete(request.params.id, function(err, dbResult) {
    if (err) {
      response.json(err);
    } else {
      response.json(dbResult);
    }
  });
});


router.put('/:id', 
function(request, response) {
  account.update(request.params.id, request.body, function(err, dbResult) {
    if (err) {
      response.json(err);
    } else {
      response.json(dbResult);
    }
  });
});

/*
router.post('/:id', 
    function(request, response) {
      account.withdrawal(request.params.id, request.body, function(err, dbResult) {
        if (err) {
          console.log(err);
          response.json(err);
        } else {
          console.log(dbResult);
          response.json(dbResult);
        }
      });
    });
*/

module.exports = router;
