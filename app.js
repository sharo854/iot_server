const express = require('express')
const mysql = require('mysql');
const cron = require('node-cron');
const app = express()

var cov = {Mon:"月", Tue:"火", Wed:"水", Tur:"木", Fri:"金", Sat:"土", Sun:"日"}
require('date-utils'); 
var dt = new Date();
dt.setHours(dt.getHours() + 6)
var timestring = dt.toFormat("M/D (") + cov[dt.toFormat("DDD")] + ") の予定";

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
		'update attendance set state_work=0;',
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

app.post('/change/state_work/:user/:id', (req, res) => {
	console.log('change');	
	connection.query(
		'update attendance set state_work=? where user=?;',
		[req.params.id, req.params.user],
		(error, results) => {
			// console.log(results);
			res.redirect('/');
		}
	);
});
app.post('/change/state_now/:user/:id', (req, res) => {
	console.log('change');	
	connection.query(
		'update attendance set state_now=? where user=?;',
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
		'SELECT state_work FROM attendance where user="てい"',
		(error, results) => {
			console.log(results);
			res.render('stateapi.ejs', {item: results[0]});
		}
	);
});

var server = app.listen(3002, () => {
	console.log('Example app listening on port ' + server.address().port)
});


