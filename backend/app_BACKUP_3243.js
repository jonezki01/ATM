var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');

var indexRouter = require('./routes/index');
<<<<<<< HEAD
var usersRouter = require('./routes/users');
var transactionRouter = require('./routes/transaction');
||||||| b2c91c3
var usersRouter = require('./routes/users');
=======
var customerRouter = require('./routes/customer');
>>>>>>> main

var app = express();

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', indexRouter);
<<<<<<< HEAD
app.use('/users', usersRouter);
app.use ('/transaction', transactionRouter);
||||||| b2c91c3
app.use('/users', usersRouter);
=======
app.use('/customer', customerRouter);
>>>>>>> main

module.exports = app;
