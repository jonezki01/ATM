const express = require('express');
const router = express.Router();
const bcrypt = require('bcryptjs');
const login = require('../models/card_models');
const jwt = require('jsonwebtoken');
const dotenv = require('dotenv');

router.post('/', 
  function(request, response) {
    if(request.body.username && request.body.password){
      const user = request.body.username; //id card int
      const pass = request.body.password; // pin
      
        login.checkpin(user, function(dbError, dbResult) {
          if(dbError){
            response.json(dbError);
          }
          else{
            if (dbResult.length > 0) {
              bcrypt.compare(pass,dbResult[0].pin, function(err,compareResult) {
                if(compareResult) {
                  console.log("success");
                  //console.log(process.env.MY_TOKEN, user);
                  const token = generateAccessToken({ username: user });
                  response.send(token);
                }
                else {
                    console.log("wrong password");
                    //console.log(user, pass);
                    //console.log(dbResult[0].pin);
                    //console.log(dbResult.length);
                    response.send(false);
                }			
              }
              );
            }
            else{
              console.log("user does not exists");
              response.send(false);
            }
          }
          }
        );
      }
    else{
      console.log("username or password missing");
      response.send(false);
    }
  }
);

function generateAccessToken(user) {
  dotenv.config();
  return jwt.sign(user, process.env.MY_TOKEN, { expiresIn: '1800s' });
}

module.exports=router;