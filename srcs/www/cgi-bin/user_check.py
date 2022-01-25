#!/usr/bin/env python3.4

import cgi, cgitb
import os
import sqlite3
import fileinput
try:
    from urllib.parse import parse_qs
except ImportError:
     from urlparse import parse_qs


cgitb.enable()
form = ""
if os.getenv('REQUEST_METHOD') == "POST":
    for line in fileinput.input():
        form += line
    form = parse_qs(form)
    if 'register_no' in form:
        register_no = form['register_no'][0]
    else:
        register_no = "none"
    username = form['username'][0]
    passwd = form['password'][0]
elif os.getenv('REQUEST_METHOD') == "GET":
    form = cgi.FieldStorage()
    if 'register_no' in form:
        register_no = form['register_no'].value
    else:
        register_no = "none"
    if 'username' in form:
        username = form['username'].value
    else:
        username = "none"
    if 'password' in form:
        passwd = form['password'].value
    else:
        passwd = "none"




print("Content-type:text/html\r\n\r\n")
print("<html>")
print("<head>")
print("</head>")
print("<body>")
print('<div style = "text-align:center ; "')
print("<h1><b>Login page</b></h1>")
## making a connection to the sqlite3 database
## Note : If you get an error while connecting to the d, try giving the absolute path to the db file
## eg : /path/cgi-bin/user_base.db
# conn = sqlite3.connect('user_base.db')
# cur = conn.cursor()

## now to check whether the entered data is for
## -> new user 
## -> an old user

query = "SELECT * FROM users"
# cur.execute(query)
# print("<p>" , cur.fetchall, "</p>")
# cur.execute('SELECT user_name FROM users WHERE reg_no = ?', (register_no,))
# rows = cur.fetchall()

## mind you that the above variable "rows" is a list and stores all the entries, grouped together in
## a row. So if the user with the particular "register_no" does not exist in the database. "rows" will
## be an empty list 

print("<br><br>")
# if len(rows) == 0:  ### if list is empty, we could also have done a "if not a:" but that is not that intuitive
#     print("<p>User : <b>", username , "</b> does not exist.</p>")
#     cur.execute('INSERT INTO users VALUES(?, ?, ?)', (register_no, username, passwd))
#     conn.commit()
#     print("<p>User was created successfully</p>")
#     print("Done")
#     print("</div>")
    
# else:
print("<p>Welcome<b>", username ,"</b>. Good to have you back")
print("<br><p>Your account details:</p>")
print("<p><b>Register number : </b>", register_no, " </p>")
print("<p><b>Username : </b> " , username, "</p>")
print("</div>")