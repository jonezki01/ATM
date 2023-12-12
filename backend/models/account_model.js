const db = require('../database');

const account = {
  getAll: function(callback) {
    return db.query('select * from account', callback);
  },
  /*withdrawal: function(id, account, callback) {
    return db.query('call withdrawal(?, ?)', [id, account.amount], callback);
  },*/
  getById: function(id, callback) {
    return db.query('select * from account where idaccount=?', [id], callback);
  },
  add: function(account, callback) {
    return db.query('insert into account (balance, credit_limit, Customer_idCustomer, type) values (?, ?, ?, ?)',
      [account.balance, account.credit_limit, account.Customer_idCustomer, account.type], callback);
  },
  delete: function(id, callback) {
    return db.query('delete from account where idaccount=?', [id], callback);
  },
  update: function(id, account, callback) {
    return db.query('update account set balance=?,credit_limit=?, Customer_idCustomer=?, type=? where idaccount=?',
      [account.balance, account.credit_limit, account.Customer_idCustomer, account.type, id], callback);
  },
}

module.exports = account;