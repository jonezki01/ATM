const db = require('../database');
const bcrypt = require('bcryptjs');

const saltRounds=10;
const card={
  getAll: function(callback) {
    return db.query('select * from card', callback);
  },
  getById: function(id, callback) {
    return db.query('select * from card where idcard=?', [id], callback);
  },
  add: function(card, callback) {
    bcrypt.hash(card.pin, saltRounds, function(err, hash) {
      return db.query('insert into card (debit, credit, multicard, pin, Customer_idCustomer) values(?,?,?,?,?)',
      [card.debit, card.credit, card.multi, hash, card.Customer_idCustomer], callback);
    });
  },
  delete: function(id, callback) {
    return db.query('delete from card where idcard=?', [id], callback);
  },
  update: function(id, card, callback) {
    bcrypt.hash(card.pin, saltRounds, function(err, hash) {
      return db.query('update card set debit=?, credit=?, multicard=?, pin=?, Customer_idCustomer=? where idcard=?',
      [card.debit, card.credit, card.multi, hash, card.Customer_idCustomer, id], callback);
    });
  },
  checkpin: function(idcard, callback){
    return db.query('select pin from card where idcard=?',[idcard],callback);
  },
  getcards: function(callback) {
    return db.query('call comboboxcard()', callback);
  },
  // calls procedure where idcard and corresponding idaccount are linked.
  /*getaccountselection: function(idcard, callback) {
    return db.query('call getaccountselection(?)',[idcard], callback);
  },
  */
  getaccountselection: function(idcard, callback) {
    db.query('call getaccountselection(?)', [idcard], function(error, results, fields) {
        if (error) {
            callback(error);
        } else {
            callback(null, JSON.stringify(results));
        }
    });
},
};
          
module.exports = card;

