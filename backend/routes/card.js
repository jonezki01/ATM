const express = require('express');
const router = express.Router();
const card = require('../models/card_models');

router.get('/',
    function (request, response) {
        card.getAll(function (err, dbResult) {
            if (err) {
                response.json(err);
            } else {
                console.log(dbResult);
                response.json(dbResult);
            }
        })
    });
    //-----for combobox population------
    router.get('/allcards/',
    function (request, response) {
        card.getcards(function (err, dbResult) {
            if (err) {
                response.json(err);
            } else {
                console.log(dbResult);
                response.json(dbResult);
            }
        })
    });
  // calls procedure where idcard and corresponding idaccount are linked.
    router.get('/getaccountselection/:idcard', function (request, response) {
      const accountId = request.params.accountId;
      card.getaccountselection(accountId, function (err, dbResult) {
        if (err) {
          response.json(err);
        } else {
          console.log(dbResult);
          response.json(dbResult);
        }
      });
    });

router.get('/:id',
    function (request, response) {
        card.getById(request.params.id, function (err, dbResult) {
            if (err) {
                response.json(err);
            } else {
                response.json(dbResult[0]);
            }
        })
    });


router.post('/', 
function(request, response) {
  card.add(request.body, function(err, dbResult) {
    if (err) {
      response.json(err);
    } else {
      response.json(request.body);
    }
  });
});


router.delete('/:id', 
function(request, response) {
  card.delete(request.params.id, function(err, dbResult) {
    if (err) {
      response.json(err);
    } else {
      response.json(dbResult);
    }
  });
});


router.put('/:id', 
function(request, response) {
  card.update(request.params.id, request.body, function(err, dbResult) {
    if (err) {
      response.json(err);
    } else {
      response.json(dbResult);
    }
  });
});


module.exports = router;
