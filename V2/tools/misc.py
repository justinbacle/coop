

def getFromDict(_dict: dict, keyList: list):
    if isinstance(_dict, dict):
        if len(keyList) == 0:
            return _dict
        elif keyList[0] in _dict.keys():
            return getFromDict(_dict[keyList[0]], keyList[1:])
        else:
            return None
    else:
        return _dict


def setInDict(dic, keys, value, create_missing=True):
    d = dic
    for key in keys[:-1]:
        if key in d:
            d = d[key]
        elif create_missing:
            d = d.setdefault(key, {})
        else:
            return dic
    if keys[-1] in d or create_missing:
        d[keys[-1]] = value
    return dic


def mergeDicts(dict1, dict2):
    for k in set(dict1.keys()).union(dict2.keys()):
        if k in dict1 and k in dict2:
            if isinstance(dict1[k], dict) and isinstance(dict2[k], dict):
                yield (k, dict(mergeDicts(dict1[k], dict2[k])))
            else:
                # If one of the values is not a dict, you can't continue merging it.
                # Value from second dict overrides one in first and we move on.
                yield (k, dict2[k])
                # Alternatively, replace this with exception raiser to alert you of value conflicts
        elif k in dict1:
            yield (k, dict1[k])
        else:
            yield (k, dict2[k])


"""
tempDict = {
    "a": {
        "b": {
            "c": 14
        }
    }
}

print(tempDict)
_ = getFromDict(tempDict, "a.b.c".split('.'))
print(_)
_ = getFromDict(tempDict, "a.b".split('.'))
print(_)
tempDict = setInDict(tempDict, "d.e".split('.'), 42)
print(tempDict)
_ = getFromDict(tempDict, "d.e".split('.'))
print(_)
"""
