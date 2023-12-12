const db = require('../database');

const withdraw = {
  post: function(account, callback) {
    return db.query('call withdrawal(?, ?, ?)', [account.cardid, account.accountid, account.sum], callback);
  },
}

module.exports = withdraw;