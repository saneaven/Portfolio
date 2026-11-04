# Database (MySQL)

*Relational database coursework: schema design, comprehensive SQL, and a JDBC-backed social-network app on MySQL.*

## Overview

A solo set of database projects built at DigiPen in 2024, progressing from raw SQL queries to relational design and finally to a working web application that runs analytics queries against a live MySQL database.

The work is organized as three assignments that build on one another:

1. **Comprehensive SQL** against a rare-books inventory schema: querying, aggregation, roll-ups, sub-queries, and multi-table modification.
2. **Entity-relationship design**: modeling an Instagram-style social service (`Imstagram`) as an ER diagram and translating it into a normalized set of tables with primary/foreign-key constraints.
3. **A JDBC web app**: a Java servlet application that connects to MySQL and answers social-graph questions (friend suggestions, potential siblings, match pairs, popular cities, etc.) over a seeded user/friends/photos dataset.

Everything here is my own work.

## Highlights

- **Reporting and aggregation in SQL.** Inventory counts by book condition, total sales by condition, best-seller lists, and a daily sales report with per-month and per-year subtotals built with `UNION ALL` roll-ups and `YEAR()`/`MONTH()`/`DAY()` date parsing.
- **Correlated sub-queries and `HAVING`.** Selecting books that sold above the average sale amount for a given month, and inserting/deleting across multiple related tables using `LAST_INSERT_ID()` to chain dependent inserts.
- **Referential integrity by design.** Every schema uses explicit `PRIMARY KEY` and `FOREIGN KEY` constraints; the social schema uses composite keys (e.g. a photo tag keyed on `(photo_id, subject_id)`) and a deferred `ALTER TABLE ... ADD CONSTRAINT` to resolve a circular album/photo reference.
- **Graph-style queries over a social dataset.** The web app answers questions such as the top pairs of non-friends with the most friends in common, users who still live in their hometown, users with no friends, the most-tagged photos, and cross-gender "match pairs" who appear together in photos: expressed as multi-way self-joins with tie-breaking `ORDER BY` rules.
- **End-to-end JDBC integration.** Java servlets open a JDBC connection, execute the analytics SQL, and render results as HTML, with query outputs also exported as CSV.

## How it works

### Assignment 1: Rare-books SQL

`Create_Rare_Books.sql` builds and seeds a bookstore inventory schema: `Author`, `Work`, `Book`, `Publisher`, `Customer`, `Sale`, `Volume`, and `Condition_Codes`, wired together with foreign keys (a `Book` belongs to a `Work` and `Publisher`; a `Volume` is a physical copy tied to a `Book`, a condition code, and optionally a `Sale`). The answer script then solves a series of reporting exercises against that data: stock counts, sales totals, inventory-by-author, publisher breakdowns, a roll-up sales report, an above-average-price filter, a best-seller list, and multi-table insert/delete operations.

### Assignment 2: Social schema design

An ER diagram (`.drawio` / `.png`) models a social platform where users sign up, record profile and education details, share photos in albums, exchange private messages, and create events. `CreateTables.sql` realizes the design as roughly a dozen tables (`User_Profile`, `Education_History`, `Location`, `Hometown_Location`/`Current_Location`, `Basic_Event`, `Event_Participants`, `Message`, `Album`, `Photo`, and `Photo_Tags`), each with appropriate keys and foreign-key constraints. An accompanying write-up explains the design decisions (a shared `Location` table, composite keys, and why certain tables omit a surrogate id).

### Assignment 3: `Imstagram` JDBC app

`table_generation.sql` creates and seeds a social dataset: `User` (with a `gender` ENUM and current/hometown city), an undirected `Friends` edge table (composite primary key on `(user1_id, user2_id)`), `Photo`, and `Tagged_photo_user`. On top of this, a Java application implements one query per question as an `HttpServlet`:

- Each servlet opens a JDBC connection to the MySQL database, runs its SQL through a `Statement`/`ResultSet`, and writes the result to the response as HTML.
- Queries cover name statistics, birth-month grouping, users living in their hometown, friendless users, popular photo cities, most-tagged photos, oldest/youngest friend lookups, potential siblings, friend suggestions by shared connections, and cross-gender match pairs.
- Servlets are mapped to URL paths in `web.xml` (for example `/SuggestFriend`, `/PopularCities`, `/MatchPairs`), with a main page servlet forwarding to a static HTML view.
- The corresponding query results are also saved as CSV files under `Output/`.

## Tech stack

- **MySQL**: schema definition (DDL) and queries (DML), including aggregation, sub-queries, `UNION ALL` roll-ups, and multi-table modification.
- **Java** with **JDBC** (`java.sql`) and the **MySQL Connector/J 8.0.32** driver for database connectivity.
- **Java Servlets** (`javax.servlet`) for the web layer, with the servlet API bundled for a Tomcat-style container.
- **drawio** for the entity-relationship diagram.

## Build & run

The SQL scripts run directly in a MySQL workbench or client: execute `Create_Rare_Books.sql` (Assignment 1), `CreateTables.sql` (Assignment 2), or `table_generation.sql` (Assignment 3) to create and seed the databases, then run the query scripts against them.

The `Imstagram` web app was developed as an IntelliJ IDEA Java module. The servlet classes connect to a local MySQL instance (`jdbc:mysql://localhost:3306/...`); update the connection URL, user, and password constants at the top of each servlet for your environment. The module depends on `mysql-connector-j-8.0.32.jar` and `servlet-api.jar` (both bundled under `WEB-INF/lib/`), and is deployed to a servlet container, where each servlet is reachable at the URL path defined in `web.xml`.

## Project structure

```
Database-MySQL/
├── Assignment1/                     # Comprehensive SQL on a rare-books schema
│   ├── Create_Rare_Books.sql        # Schema + seed data
│   └── Assignment_1_*.sql           # Query/report exercises
├── Assignment2/                     # ER design for a social platform
│   ├── *.drawio / *.png             # Entity-relationship diagram
│   ├── *_CreateTables.sql           # DDL for the social schema
│   ├── *_dropTables.sql
│   └── *_er_imstagram_explain.docx  # Design write-up
└── Assignment3/                     # Imstagram JDBC web app
    ├── Deliverables/
    │   ├── table_generation.sql     # User / Friends / Photo / tag tables + seed data
    │   └── Output/                  # Query results exported as CSV
    └── Imstagram/
        └── src/                     # Java servlets (one query per servlet) + web.xml
```

## Context

DigiPen, 2024. Solo database coursework.
