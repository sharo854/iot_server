const express = require('express')
const mysql = require('mysql');
const cron = require('node-cron');
const app = express()

require('date-utils'); 
var dt = new Date();
var timestring = dt.toFormat("M/D (DDD) の予定")

app.use(express.static('public'))

const connection = mysql.createConnection({
  host: 'localhost',
  user: 'root',
  password: '108379',
  database: 'charlotte'
});

connection.connect((err) => {
  if (err) {
    console.log('error connecting: ' + err.stack);
    return;
  }
  console.log('success');
});

cron.schedule('0 51 21 * * *', () => {
	connection.query(
		'update attendance set state=3;',
		(error, results) => {
			console.log("リセット");
		}
	);
});


app.get('/', (req, res) => {
	connection.query(
		'SELECT * FROM attendance',
		(error, results) => {
			console.log(results);
			res.render('main.ejs', {items: results, timestr: timestring});
		}
	);
});

app.post('/change/:user/:id', (req, res) => {
	console.log('change');	
	connection.query(
		'update attendance set state=? where user=?;',
		[req.params.id, req.params.user],
		(error, results) => {
			// console.log(results);
			res.redirect('/');
		}
	);
});

app.get('/api/v1/test', (req, res) => {
	console.log('test');
	res.writeHead(200, {'Content-Type': 'text/plain;charset=utf-8;'});
	res.write("これはテストページです。");
	res.end();
});

app.get('/api/v1/state', (req, res) => {
	console.log('api/state');	
	connection.query(
		'SELECT state FROM attendance where user="てい"',
		(error, results) => {
			console.log(results);
			res.render('stateapi.ejs', {item: results[0]});
		}
	);
});

var server = app.listen(3002, () => {
	console.log('Example app listening on port ' + server.address().port)
});


