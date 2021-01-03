import json


def readJson(path: str = ""):
    with open(path, 'r') as f:
        jsonDict = json.load(f)
    return jsonDict
