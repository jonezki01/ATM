const db = require('../database');

const sharedaccounts = {
    post: function(sharedaccounts, callback) {
        return db.query('call getsharedaccountsbycardid(?);', [sharedaccounts.idcard], callback);
    },
}

module.exports = sharedaccounts;