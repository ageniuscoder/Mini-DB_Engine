# 🧩 Astra Query Language (Astra QL)

Astra QL is a lightweight, SQL-like query language for managing databases, tables, and data inside the **Astra** system. It supports creating databases and tables, inserting and querying data, performing updates, removing records, and managing server connections.

---

## 📜 Commands Overview

| Category | Commands |
|-----------|-----------|
| **Database-level** | `create`, `use`, `export` |
| **Table-level** | `new`, `add` |
| **Data-level** | `print`, `remove`, `update` |
| **Server-level** | `server socket_create`, `server socket_connect` |
| **Utility** | `exit` |

---

## 🏗️ Database Commands

### 1️⃣ `create`
Create a new database.

```dql
create <database_name>
eg : create college
```


### 2️⃣ `use`
Switch to an existing database.

```dql
use <database_name>
eg : use college
```

### 3️⃣ `export`
Export the current database to a file.

```dql
export <file_name>
eg : export college
```

## 🧱 Table Commands

### 4️⃣ `new`
Create a new table in the current database.

```dql
new <table_name> :: [primary] <type> <column_name>[, ...]
eg : new students :: primary int id, string 20 name, float gpa
```

### 5️⃣ `add`
Add a column to an existing table.

```dql
add <table_name> :: [ <val1>, <val2>, ... ], [ <val1>, <val2>, ... ]
eg : add students :: [1, "lakshay", 8.6], [2, "mangal", 8.9]
```

## 📊 Data Commands

### 6️⃣ `print`
Print the contents of a table.

```dql
print <table_name> [:: <condition>]
eg : print students
print students :: gpa > 8.5 && name == "bob"
```

### 7️⃣ `remove`
Remove a row from a table.

```dql
remove <table_name> [:: <condition>]
eg : remove students :: id == 2
```

### 8️⃣ `update`
Update a row in a table.

```dql
update <table_name> [:: <condition>] -> <field=value>[, <field=value>, ...]
update students -> gpa = 9.8
update students :: id == 1 -> name = "lakshay", gpa = 8.62
```

## 🌐 Server Commands

### 9️⃣ `server socket_create`
Create a server socket.

```dql
server socket_create <float_data> : <int_data>
eg : server socket_create 127.0 : 8080
```

### 10️⃣ `server socket_connect`
Connect to a server socket.

```dql
server socket_connect <float_data> : <int_data>
eg : server socket_connect 127.0 : 8080
```

## 🛠️ Utility Commands

### 11️⃣ `exit`
Exit the DQL shell.

```dql
exit
```
---