const express = require('express')
const mysql = require('mysql');
const app = express()

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

app.get('/', (req, res) => {
	connection.query(
		'SELECT state FROM attendance',
		(error, results) => {
			console.log(results);
			res.render('main.ejs', {item: results[0]});
		}
	);
});

app.post('/change1', (req, res) => {
	console.log('change');	
	connection.query(
		'update attendance set state=1;',
		(error, results) => {
			console.log(results);
			res.redirect('/');
		}
	);
});
app.post('/change2', (req, res) => {
	console.log('change');	
	connection.query(
		'update attendance set state=2;',
		(error, results) => {
			console.log(results);
			res.redirect('/');
		}
	);
});
app.post('/change3', (req, res) => {
	console.log('change');	
	connection.query(
		'update attendance set state=3;',
		(error, results) => {
			console.log(results);
			res.redirect('/');
		}
	);
});
app.post('/change/:id', (req, res) => {
	console.log('change');	
	connection.query(
		'update attendance set state=?;',
		[req.params.id],
		(error, results) => {
			console.log(results);
			res.redirect('/');
		}
	);
});


app.get('/top', (req, res) => {
	res.render('top.ejs');
});

var server = app.listen(3001, () => {
	console.log('Example app listening on port ' + server.address().port)
});
