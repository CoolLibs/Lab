-- Import the luasocket library
local socket = require("socket")

-- Use the socket library
local ip = socket.dns.toip("www.google.com")
Coollab.log("Google's IP is: " .. ip)
