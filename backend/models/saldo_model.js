const db = require('../database');

const saldo = {
    post: function(saldo, callback) {
        return db.query('select balance from account where idaccount = ?;',
        [saldo.accountid], callback);
    },
}

module.exports = saldo;