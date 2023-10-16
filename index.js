const express = require('express');
const { exec } = require('child_process');

const app = express();
const port = 3000;

app.use(express.json());

app.get('/', (req, res) => {
  res.json('Home');
});

app.get('/c', (req, res) => {
  console.time("CódigoC")
  
  const { param1, param2 } = req.body
  
  exec(`./src/build/soma ${param1} ${param2}`, (error, stdout, stderr) => {
    if (error) {
      res.json({ error: stderr});
      return
    }

    res.json({ message: stdout});
  });

  console.timeEnd("CódigoC")
});

app.get('/js', (req, res) => {
  console.time("CódigoJS")
  
  const { param1, param2 } = req.body

  exec(`node ./src/build/soma.js ${param1} ${param2}`, (error, stdout, stderr) => {
    if (error) {
      res.json({ error: stderr});
      return
    }

    res.json({ message: stdout});
  });

  console.timeEnd("CódigoJS")
});

// User Routes
app.post('/user/login', (req, res) => {
  res.json('Login');
});

app.post('/user/register', (req, res) => {
  res.json('Register');
});

// Note Routes
app.post('/note/create', (req, res) => {
  res.json('Note Created');
});

app.get('/note/read', (req, res) => {
  res.json('Read All Notes');
});

app.get('/note/:id', (req, res) => {
  res.json(`Read Note ${req.params.id}`);
});

app.put('/note/:id', (req, res) => {
  res.json(`Note ${req.params.id} updated`);
});

app.delete('/note/:id', (req, res) => {
  res.json(`Note ${req.params.id} deleted`);
});

// About Route
app.get('/about', (req, res) => {
  res.json('About Page');
});

app.listen(port, '0.0.0.0', () => {
  console.log(`API rodando em http://localhost:${port}`);
});
