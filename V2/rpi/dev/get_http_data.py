import urllib.request
import re
# from typing import Tuple  # not compatible w/ 3.7

ip = "192.168.1.111"


# def getSensorDataFromIp(ip: str) -> Tuple[str, float]:  # not compatible w/ 3.7
def getSensorDataFromIp(ip):
    if not re.match(ip, r"http:\\\\.*"):
        ip = 'http://' + ip

    response = urllib.request.urlopen(ip)
    html = response.read().decode()
    print(f"got response : {html}")

    _ = re.search(r"<h1>(\w*)</h1><h2>(\d+\.\d+)</h2>", html)

    # assuming only one sensor data
    sensor = _.group(1)
    temp = _.group(2)
    temp = float(temp)

    return (sensor, temp)


print(getSensorDataFromIp(ip))
