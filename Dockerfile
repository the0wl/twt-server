# Use an official base image, e.g., a version of Ubuntu
FROM ubuntu:latest

# Install Node.js
RUN apt-get update && apt-get install -y nodejs npm

# Install sqlite3 for C
RUN apt-get install -y sqlite3 libsqlite3-dev

# Copy your C and JavaScript source files into the container
COPY . /usr/src/twt-server

# Set the working directory
WORKDIR /usr/src/twt-server

# Compile your C program
RUN gcc ./src/core/main.c -lsqlite3 -o src/build/main
RUN gcc ./src/core/migrations/01_create-table-user.c -lsqlite3 -o src/build/migrations/01_create-table-user
RUN gcc ./src/core/migrations/02_create-table-note.c -lsqlite3 -o src/build/migrations/02_create-table-note
RUN gcc ./src/core/migrations/03_create-user-data.c -lsqlite3 -o src/build/migrations/03_create-user-data
RUN gcc ./src/core/migrations/04_create-note-data.c -lsqlite3 -o src/build/migrations/04_create-note-data
RUN ./src/build/migrations/01_create-table-user
RUN ./src/build/migrations/02_create-table-note
RUN ./src/build/migrations/03_create-user-data
RUN ./src/build/migrations/04_create-note-data

# Install JavaScript dependencies
# Assuming you have a 'package.json' file
RUN npm install