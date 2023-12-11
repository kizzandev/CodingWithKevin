import * as express from 'express';
import * as fs from 'node:fs';
import * as crypto from 'node:crypto';

const app = express();

function readEnv(path = '.env') {
  try {
    const env = fs.readFileSync(path, 'utf8');
    const lines = env
      .replace(/[\r]/gm, '')
      .split('\n')
      .filter((line) => line.trim() !== '' && !line.startsWith('#'));

    lines.forEach((line) => {
      const [key, ...val] = line.split('=');
      const value = val.join('=');

      const hasQuotes = value.startsWith('"') && value.endsWith('"');

      const valueToStore = hasQuotes ? value.slice(1, -1) : value;

      process.env[key] = valueToStore;
    });
  } catch (error) {
    console.log('Error: ', error);
  }
}
readEnv();

const URL = process.env.ENV_LOCAL_URL || '';
const PORT = process.env.ENV_LOCAL_PORT || 3000;

type Task = {
  id: string;
  name: string;
  done: boolean;
};

app.use(express.json());

app.use(function (req, res, next) {
  res.setHeader(
    'Access-Control-Allow-Headers',
    'X-Requested-With,content-type'
  );
  res.setHeader('Access-Control-Allow-Origin', `${URL}:${PORT}`);
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, PUT, DELETE');
  next();
});

app.get('/', (req, res) => {
  res.sendFile(__dirname + '/index.html');
});

// Define routes for CRUD operations
app.get('/tasks', async (req, res) => {
  const data = JSON.parse(fs.readFileSync('./tasks.json').toString());

  if (data) {
    res.json({ tasks: data });
  } else {
    res.status(500).json('Could not read data.');
  }
});

app.post('/tasks', (req, res) => {
  const data: Array<Task> = JSON.parse(
    fs.readFileSync('./tasks.json').toString()
  );

  let newId = crypto.randomBytes(16).toString('hex');
  newId
    .replace('\\', '')
    .replace('/', '')
    .replace(/[?].[=]/, '');

  const newTask = req.body;
  newTask.id = newId;

  data.push(req.body);
  fs.writeFileSync('./tasks.json', JSON.stringify(data));
  res.status(201).json('Task added successfully!');
});

app.put('/tasks/:id', (req, res) => {
  const data: Array<Task> = JSON.parse(
    fs.readFileSync('./tasks.json').toString()
  );

  data.forEach((task) => {
    if (task.id === req.params.id) task.done = !task.done;
  });

  fs.writeFileSync('./tasks.json', JSON.stringify(data));
  res.status(201).json('Task updated successfully!');
});

app.delete('/tasks/:id', (req, res) => {
  const data: Array<Task> = JSON.parse(
    fs.readFileSync('./tasks.json').toString()
  );
  const newData = data.filter((task) => task.id !== req.params.id);
  fs.writeFileSync('./tasks.json', JSON.stringify(newData));
  res.status(203).json('Task deleted succesfully!');
});

app.listen(PORT, () => console.log(`Server listening at ${URL + ':'}${PORT}`));
