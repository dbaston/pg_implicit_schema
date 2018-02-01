# Implicit Schema Creation for PostgreSQL

## What is this?

This extension is a tiny hack that causes Postgres to automatically generate a
schema whenever you try to create an object inside it, e.g.:

`CREATE TABLE fizz.fuzz (id int);`

will be implicitly rewritten as:

`CREATE SCHEMA IF NOT EXISTS fizz; CREATE TABLE fizz.fuzz (id int);`

## Why do I want this?

Well, you probably don't.
But here's why I did:

* A web service is accepting requests that, among other things, specify a
  schema and table for an object to be created. The schema name includes a
  timestamp, so all possible values can't be created beforehand.
* The database is configured to accept connections only from the web service,
  so you can't go in and create the schemas yourself.
* The code for the web service, alas, cannot be modified.

