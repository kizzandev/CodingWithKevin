# Todo Backend Project

## Description
This is a simple ToDo backend project built with Node.js, express.js and TypeScript.

## Setup
1. Install dependencies:
   ```bash
   npm install
   ```

2. Build and Start the server:
   ```bash
   npm run dev
   ```

## API Endpoints

- `GET /tasks`: Get all tasks.
- `POST /tasks`: Create a new todo.
  ```json
  { "name": "Example Todo", "done": false }
  ```

- `PUT /tasks/:id`: Update a todo by ID.
  ```json
  { "name": "Updated Todo", "done": true }
  ```

- `DELETE /tasks/:id`: Delete a todo by ID.

## Environment Variables
1. Duplicate the `.env.sample` file:
   ```bash
   cp .env.sample .env
   ```

2. Edit the `.env` file and set values according to your environment:
   ```env
   ENV_LOCAL_URL=127.0.0.1
   ENV_LOCAL_PORT=3000
   ```
   
   Replace the default values with your desired configuration.
