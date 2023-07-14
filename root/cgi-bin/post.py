#!/usr/bin/env python3
import cgi

# Retrieve the form data
form = cgi.FieldStorage()
text = form.getvalue('text', '')

# Print the HTTP response headers
print("Content-type: text/html\r\n\r\n")

# Print the HTML page
print("""
<!DOCTYPE html>
<html>
<head>
    <title>CGI Form Example</title>
</head>
<body>
    <h1>CGI Form Example</h1>
    <form method="post" action="">
        <label for="text">Enter Text:</label>
        <input type="text" name="text" id="text" value="{0}">
        <button type="submit">Submit</button>
    </form>
    <h2>Text Entered:</h2>
    <p>{0}</p>
</body>
</html>
""".format(text))