const db = require('../database');

const transaction = {
  getAll: function(callback) {
    return db.query('select * from transaction', callback);
  },
  getById: function(id, callback) {
    return db.query('select * from transaction where idtransaction=?', [id], callback);
  },
  add: function(transaction, callback) {
    return db.query(
      'insert into transaction (action, sum, cardaccountid) values(?,?,(select idcardhasaccount from card_has_account where card_idcard=? and account_idaccount=?))',
      [transaction.action, transaction.sum, transaction.cardused, transaction.accountused],
      callback
      
    );
  },
  delete: function(id, callback) {
    return db.query('delete from transaction where idtransaction=?', [id], callback);
  },
  update: function(id, transaction, callback) {
    return db.query(
      'update transaction set action=?,sum=? where idtransaction=?',
      [transaction.action, transaction.sum, id],
      callback
    );
  }
};
module.exports = transaction;
