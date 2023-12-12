const mysql = require('mysql2');
const connection = mysql.createPool({
  host: '127.0.0.1',
  user: 'bankuser',
  port: '3307',
  password: 'bankpass',
  database: 'bank_simul_r14'
});
module.exports = connection;
