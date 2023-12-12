const db = require('../database');

const selectaccount = {
  post: function(selectaccount, callback) {
    return db.query('call getaccountselection(?);', [selectaccount.idcard], callback);
  },
}

module.exports = selectaccount;