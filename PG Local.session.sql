CREATE TABLE IF NOT EXISTS users (
    Id serial primary key,
    userName varchar(255) NOT NULL,
    email varchar(255) UNIQUE NOT NULL,
    password varchar(255) NOT NULL
);