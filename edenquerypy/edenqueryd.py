import os
import sys
import time
import logging
from logging.handlers import TimedRotatingFileHandler
import signal

import edenpy

EDEN_LOG_FILE = '/tmp/edenquerydaemon.log'
EDEN_PID_FILE = '/tmp/edenquerydaemon.pid'
EDEN_BACKUP_COUNT = 10

# Set up logging with rotation
log_handler = TimedRotatingFileHandler(EDEN_LOG_FILE, when='midnight', backupCount=EDEN_BACKUP_COUNT)
log_handler.setFormatter(logging.Formatter('%(asctime)s %(message)s'))
logger = logging.getLogger()
logger.setLevel(logging.INFO)
logger.addHandler(log_handler)
# logging.basicConfig(filename='/tmp/edenquerydaemon.log', level=logging.INFO, format='%(asctime)s %(message)s')

def daemonize():
    try:
        pid = os.fork()
        if pid > 0:
            sys.exit(0)
    except OSError as e:
        sys.stderr.write(f"Fork #1 failed: {e.errno} ({e.strerror})\n")
        sys.exit(1)

    os.chdir('/')
    os.setsid()
    os.umask(0)

    try:
        pid = os.fork()
        if pid > 0:
            sys.exit(0)
    except OSError as e:
        sys.stderr.write(f"Fork #2 failed: {e.errno} ({e.strerror})\n")
        sys.exit(1)

    sys.stdout.flush()
    sys.stderr.flush()
    with open('/dev/null', 'r') as f:
        os.dup2(f.fileno(), sys.stdin.fileno())
    with open('/dev/null', 'a+') as f:
        os.dup2(f.fileno(), sys.stdout.fileno())
    with open('/dev/null', 'a+') as f:
        os.dup2(f.fileno(), sys.stderr.fileno())

    with open(EDEN_PID_FILE, 'w') as f:
        f.write(str(os.getpid()) + '\n')

def signal_handler(signum, frame):
    logging.info("Received signal %d, exiting...", signum)
    os.remove(EDEN_PID_FILE)
    sys.exit(0)

def do_something():
    logging.info(f"Eden Py: {edenpy.edenpy_version()}")
    logging.info(f"Eden Core: {edenpy.eden_core_version()}")
    logging.info(f"Eden Analytics: {edenpy.eden_analytics_version()}")

    while True:
        logging.info(f"edenquery daemon is alive! (f={edenpy.factorial(10)})")
        time.sleep(10)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} start|stop")
        sys.exit(1)

    if sys.argv[1] == "start":
        daemonize()
        signal.signal(signal.SIGTERM, signal_handler)
        signal.signal(signal.SIGINT, signal_handler)
        do_something()
    elif sys.argv[1] == "stop":
        try:
            with open(EDEN_PID_FILE, 'r') as f:
                pid = int(f.read().strip())
            os.kill(pid, signal.SIGTERM)
        except Exception as e:
            print(f"Error stopping edenquery daemon: {e}")
            sys.exit(1)
    else:
        print(f"Unknown command: {sys.argv[1]}")
        sys.exit(1)
