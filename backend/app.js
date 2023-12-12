var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var logger = require('morgan');
const jwt = require('jsonwebtoken');

var indexRouter = require('./routes/index');
var customerRouter = require('./routes/customer');
var card_has_accountRouter = require('./routes/card_has_account');
var accountRouter = require('./routes/account');
var cardRouter = require('./routes/card');
var transactionRouter = require('./routes/transaction');
var loginRouter = require('./routes/login');
var viewtransactionsRouter = require('./routes/viewtransactions');
var withdrawRouter = require('./routes/withdraw');
var selectaccountRouter = require('./routes/selectaccount');
var saldoRouter = require('./routes/saldo');
var sharedaccountsRouter = require('./routes/sharedaccounts');

var app = express();

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/login', loginRouter);
app.use('/card', cardRouter);
//app.use(authenticateToken);
app.use('/saldo', saldoRouter);
app.use('/account', accountRouter);
app.use('/withdraw', withdrawRouter);
app.use('/viewtransactions', viewtransactionsRouter);
app.use('/', indexRouter);
app.use('/customer', customerRouter);
app.use('/card_has_account', card_has_accountRouter);
app.use ('/transaction', transactionRouter);
app.use('/selectaccount', selectaccountRouter);
app.use('/sharedaccounts', sharedaccountsRouter);

function authenticateToken(req, res, next) {
    const authHeader = req.headers['authorization']
    const token = authHeader && authHeader.split(' ')[1]
  
    console.log("token = "+token);
    if (token == null) return res.sendStatus(401)
  
    jwt.verify(token, process.env.MY_TOKEN, (err, user) => {
      console.log(err)
  
      if (err) return res.sendStatus(403)
  
      req.user = user
  
      next()
    })
  }

module.exports = app;
