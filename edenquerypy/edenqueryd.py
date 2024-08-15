import os
import sys
import time
import logging
from logging.handlers import TimedRotatingFileHandler
import signal

import requests

import edenpy
# import edenpy.myenv

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


class EdenDataLoader():

    @staticmethod
    def load_task_definitions():
        try:
            url = 'http://127.0.0.1:8000/v1/api/apiGetTaskDefinitions'
            print(url)
            response = requests.get(url)
            data = response.json()
            return data
        except requests.exceptions.HTTPError as http_err:
            print(f"HTTP error occurred: {http_err}")
        except requests.exceptions.RequestException as req_err:
            print(f"Request error occurred: {req_err}")
        except ValueError as json_err:
            print(f"JSON decode error: {json_err}")

        return None
    
    @staticmethod
    def load_tasks_from_graph(name: str):
        try:
            url = 'http://127.0.0.1:8000/v1/api/apiGetTaskFromGraph'
            url = f'{url}?name={name}'
            print(url)
            response = requests.get(url)
            data = response.json()
            return data
        except requests.exceptions.HTTPError as http_err:
            print(f"HTTP error occurred: {http_err}")
        except requests.exceptions.RequestException as req_err:
            print(f"Request error occurred: {req_err}")
        except ValueError as json_err:
            print(f"JSON decode error: {json_err}")

        return None

    @staticmethod
    def load_workflow_tasks(status: str):
        url = 'http://127.0.0.1:8000/v1/api/apiGetWorkflowTasks'
        url = f'{url}?task_status={status}'
        logging.info(url)
        try:
            response = requests.get(url)
            data = response.json()
            return data
        except requests.exceptions.HTTPError as http_err:
            print(f"HTTP error occurred: {http_err}")
        except requests.exceptions.RequestException as req_err:
            print(f"Request error occurred: {req_err}")
        except ValueError as json_err:
            print(f"JSON decode error: {json_err}")

        return None


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

def process_task(task):
    """
    wTaskID': 12345, 
    'task_id': 1, 
    'taskStatus_id': 'Waiting', 
    'priority': 4000, 
    'creationDate': '2024-07-16T21:39:49.029589Z', 
    'lastUpdateDate': '2024-07-16T21:39:49.029592Z'
    """
    # name = task_data['name']
    # desc = task_data['desc']
    logging.info(task)
    # edenpy.calibrate()

def do_something():
    logging.info(f"edenquery daemon is alive!")
    logging.info(f"Eden Py: {edenpy.edenpy_version()}")
    logging.info(f"Eden Core: {edenpy.eden_core_version()}")
    logging.info(f"Eden Analytics: {edenpy.eden_analytics_version()}")

    # print('Loading Task Definitions')
    # data = EdenDataLoader.load_task_definitions()
    # if data is not None:
    #     [logging.info(x) for x in data['response']]
    # else:
    #     logging.info("No task definitions found")
    
    # data = EdenDataLoader.load_tasks_from_graph('Test Graph PV Calculation')
    # if data is not None:
    #     process_graph(data)
    # else:
    #     logging.info("No requests found")

    while True:
        logging.info("Waiting for requests...")
        time.sleep(1)

        data = EdenDataLoader.load_workflow_tasks('Waiting')
        if data is not None:
            [process_task(task) for task in data['response']]
        else:
            logging.info("No requests found")

# Main entry of the daemon
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
