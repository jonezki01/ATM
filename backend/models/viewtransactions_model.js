const db = require('../database');

const viewtransactions = {
  getById: function(viewtransactions, callback) {
    return db.query('call viewtransactions(?,?,?)',
    [viewtransactions.cardid, viewtransactions.accountid, viewtransactions.offsetti], callback);
  },
}

module.exports = viewtransactions;