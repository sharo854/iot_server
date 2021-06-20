const express = require('express')
const mysql = require('mysql');
const cron = require('node-cron');
const app = express()

var cov = {Sun:7, Mon:1, Tue:2, Wed:3, Tur:4, Fri:5, Sat:6}
var cov_kan = ["日", "月", "火", "水", "木", "金", "土", "日"]
require('date-utils'); 

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

cron.schedule('0 00 18 * * 1-5', () => {
	connection.query(
		'update attendance set state_work=0;',
		(error, results) => {
			console.log("リセット");
		}
	);
});


app.get('/', (req, res) => {
	
	let dt = new Date();
	dt.add({"hours": 6});
	if(6==cov[dt.toFormat("DDD")]) {
		dt.add({"days": 2});
	}
	if(cov[dt.toFormat("DDD")]==7) {
		dt.add({"days": 2});
	}
	let timestring = dt.toFormat("M/D (") + cov_kan[cov[dt.toFormat("DDD")]] + ")";
	console.log(timestring);


	connection.query(
		'SELECT * FROM attendance',
		(error, results) => {
			console.log(results);
			res.render('main.ejs', {items: results, timestr: dt.toFormat("M/D (") + cov_kan[cov[dt.toFormat("DDD")]] + ")"});
		}
	);
});

app.get('/user/:user', (req, res) => {
	connection.query(
		'SELECT * FROM attendance',
		(error, results) => {
			console.log(results);
			res.render('main.ejs', {items: results, timestr: timestring, current_user: req.params.user});
		}
	);
});

// app.post('/change/:col/:user/:id', (req, res) => {
// 	console.log('change');	
// 	connection.query(
// 		'update attendance set ?=? where user=?;',
// 		[req.params.col, req.params.id, req.params.user],
// 		(error, results) => {
// 			// console.log(results);
// 			res.redirect('/');
// 		}
// 	);
// });
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


