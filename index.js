const express = require('express');
const { exec } = require('child_process');
const cors = require('cors');
const { exit } = require('process');
const fs = require('fs');

const app = express();
const port = 1337;

function meuMiddleware(req, res, next) {
  console.log(`${req.method} ${req.originalUrl}`);

  // Chame next() para passar para o próximo middleware ou para a rota final
  next();
}

app.use(express.json());
app.use(cors());
app.use(meuMiddleware);

app.get('/', (req, res) => {
  res.json('Home');
});

// User Routes
app.post('/user/login', (req, res) => {
  const { username, password } = req.body

  exec(`./src/build/main 1 '${username}' '${password}'`, (error, stdout, stderr) => {
    if (error) {
      res.status(400).json({ error: stderr});
      return
    }

    if (stdout === "404") {
      res.status(404).json()
    } else {
      const userData = stdout.split(';')
      res.status(200).json({ 
        id: userData[0],
        username: userData[1],
        name: userData[2],
        avatarUrl: userData[3],
        access_token: userData[0],
      });
    }
  });
});

app.get('/user/recover', (req, res) => {
  const { access_token } = req.headers
  
  exec(`./src/build/main 2 '${access_token}'`, (error, stdout, stderr) => {
    if (error) {
      res.status(400).json({ error: stderr});
      return
    }

    if (stdout === "404") {
      res.status(404).json()
    } else {
      const userData = stdout.split(';')
      res.status(200).json({ 
        id: userData[0],
        username: userData[1],
        name: userData[2],
        avatarUrl: userData[3],
        access_token: userData[0],
      });
    }
  });
});

app.post('/user/register', (req, res) => {
  const { name, username, password } = req.body
  
  exec(`./src/build/main 3 '${name}' '${username}' '${password}'`, (error, stdout, stderr) => {
    if (error) {
      res.status(400).json({ error: stderr});
      return
    }

    if (stdout === "401") {
      res.status(401).json()
    } else if (stdout === "404") {
      res.status(404).json()
    } else {
      const userData = stdout.split(';')
      res.status(200).json({ 
        id: userData[0],
        username: userData[1],
        name: userData[2],
        avatarUrl: userData[3],
        access_token: userData[0],
      });
    }
  });
});

// Note Routes
app.post('/note/create', (req, res) => {
  const { access_token, user_id, title, content } = req.body
  
  exec(`./src/build/main 4 '${access_token}' '${user_id}' '${title}' '${content}'`, (error, stdout, stderr) => {
    if (error) {
      res.status(400).json({ error: stderr});
      return
    }

    if (stdout === "401") {
      res.status(401).json()
    } else if (stdout === "404") {
      res.status(404).json()
    } else {
      const userData = stdout.split(';')
      res.status(200).json({ 
        id: userData[0],
        username: userData[1],
        name: userData[2],
        avatarUrl: userData[3],
        access_token: userData[0],
      });
    }
  });
});

app.get('/note/read', (req, res) => {
  const { access_token, user_id } = req.headers

  exec(`./src/build/main 5 '${access_token}' '${user_id}'`, (error, stdout, stderr) => {
    if (error) {
      res.status(400).json({ error: stderr});
      return
    }

    if (stdout === "401") {
      res.status(401).json()
    } else if (stdout === "404") {
      res.status(404).json()
    } else {
      const lines = stdout.split('\n').filter(line => line !== '')
      
      const response = lines.map(line => {
        const noteData = line.split(';')

        return { 
          id: noteData[0],
          title: noteData[1],
          content: noteData[2]
        }
      })

      res.status(200).json(response);
    }
  });
});

app.get('/note/:id', (req, res) => {
  const { id } = req.params
  const { access_token, user_id} = req.headers
  
  exec(`./src/build/main 6 '${access_token}' '${user_id}' '${id}'`, (error, stdout, stderr) => {
    if (error) {
      res.status(400).json({ error: stderr});
      return
    }

    if (stdout === "404") {
      res.status(404).json()
    } else {
      const noteData = stdout.split(';')

      if (noteData[3] != user_id) {
        res.status(401).json()
      } else {
        res.status(200).json({ 
          id: noteData[0],
          title: noteData[1],
          content: noteData[2]
        });
      }
    }
  });
});

app.put('/note/:id', (req, res) => {
  const { id } = req.params
  const { access_token, user_id, title, content } = req.headers

  exec(`./src/build/main 7 '${access_token}' '${user_id}' '${id}' '${title}' '${content}'`, (error, stdout, stderr) => {
    if (error) {
      res.status(400).json({ error: stderr});
      return
    }

    if (stdout === "404") {
      res.status(404).json()
    } else {
      const noteData = stdout.split(';')

      console.log(JSON.stringify({
        id: noteData[0],
        title: noteData[1],
        content: noteData[2]
      }))

      if (noteData[3] != user_id) {
        res.status(401).json()
      } else {
        res.status(200).json({ 
          id: noteData[0],
          title: noteData[1],
          content: noteData[2]
        });
      }
    }
  });

  res.json({ id, type: "1", title:"Primeira anotação editada", content: "Teste 1"});
});

app.delete('/note/:id', (req, res) => {
  const { id } = req.params
  const { access_token, user_id} = req.headers
  
  exec(`./src/build/main 8 '${access_token}' '${user_id}' '${id}'`, (error, stdout, stderr) => {
    if (error) {
      res.status(400).json({ error: stderr});
      return
    }

    if (stdout === "201") {
      res.status(201).json()
    } else if (stdout === "401") {
      res.status(401).json()
    } else {
      res.status(404).json()
    }
  });
});

// About Route
app.get('/about', (req, res) => {
  exec(`./src/build/main 9`, (error, stdout, stderr) => {
    if (error) {
      res.status(400).json({ error: stderr});
      return
    }

    if (stdout === "404") {
      res.status(404).json()
    } else {
      const userData = stdout.split(';')

      res.status(200).json({
        integrantes: userData[0],
        funcao: userData[1],
        data: userData[2],
      })
    }
  });
});

function init() {
  console.log("\n  Inicializando banco de dados...");

  const caminhoDoArquivo = 'src/db/note.db';

  // Verifica se o arquivo existe
  fs.access(caminhoDoArquivo, fs.constants.F_OK, (err) => {
    if (err) {
      console.log("  Não há um banco de dados.")
      console.log("  - Executando [6] migrations...")

      exec(`gcc ./src/core/main.c -lsqlite3 -o src/build/main
        gcc ./src/core/migrations/01_create-table-user.c -lsqlite3 -o src/build/migrations/01_create-table-user
        gcc ./src/core/migrations/02_create-table-note.c -lsqlite3 -o src/build/migrations/02_create-table-note
        gcc ./src/core/migrations/03_create-user-data.c -lsqlite3 -o src/build/migrations/03_create-user-data
        gcc ./src/core/migrations/04_create-note-data.c -lsqlite3 -o src/build/migrations/04_create-note-data
        ./src/build/migrations/01_create-table-user
        ./src/build/migrations/02_create-table-note
        ./src/build/migrations/03_create-user-data
        ./src/build/migrations/04_create-note-data`, (error, stdout, stderr) => {
          if (error) {
            console.log(stderr);
            exit()
          }
      
          console.log("  Banco de dados inicializado!\n")
          console.log('----------------------------------------\n');
      });
    } else {
      console.log("  Banco de dados inicializado!\n")
      console.log('----------------------------------------\n');
    }
  });
}

app.listen(port, '0.0.0.0', () => {
  console.log('\n----------------------------------------\n');
  console.log(`  API rodando em http://localhost:${port}`);
  
  init()
});
