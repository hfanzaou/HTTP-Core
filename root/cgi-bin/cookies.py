#!/usr/bin/env python3
import os
import http.cookies

# Function to get the stored cookies from the client request
def get_cookies():
    cookie = os.environ.get('HTTP_COOKIE', '')
    return http.cookies.SimpleCookie(cookie)

# Function to set a new cookie in the response
def set_cookie(name, value, max_age=None):
    cookie = http.cookies.SimpleCookie()
    cookie[name] = value

    if max_age is not None:
        cookie[name]['max-age'] = max_age

    print(cookie.output())

# Main CGI script
def main():
    # Retrieve stored cookies from the client request
    cookies = get_cookies()

    # Check if a specific cookie is already set
    if 'username' in cookies:
        username = cookies['username'].value
    else:
        username = None

    # If the user is not logged in, check login credentials
    if username is None:
        # Check login form submission
        form_data = os.environ['HTTP_COOKIE']
        form_values = dict(qc.split("=") for qc in form_data.split("&"))

        if 'username' in form_values and 'password' in form_values:
            # Simulate authentication (you should validate credentials against a database)
            if form_values['username'] == 'john_doe' and form_values['password'] == 'password123':
                # Set a new cookie for successful login
                set_cookie('username', form_values['username'], max_age=3600)  # Max-Age: 1 hour
                username = form_values['username']
            else:
                # Invalid credentials; show login page again
                print("Content-Type: text/html\n")
                with open('login.html', 'r') as login_page:
                    print(login_page.read())
                return

    # Print the response
    print("Content-Type: text/html\n")
    print("<html><body>")
    if username:
        print(f"<p>Welcome back, {username}!</p>")
    else:
        with open('login.html', 'r') as login_page:
            print(login_page.read())
    print("</body></html>")

if __name__ == '__main__':
    main()