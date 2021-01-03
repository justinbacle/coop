import os
import sys

sys.path.append(os.getcwd())
from V2.tools import misc  # noqa E402
from V2.tools import filesystem  # noqa E402


class mode(dict):

    _REQUIRED_FIELDS = [
        "id",
        "name",
        "on.condition",
        "on.delay",
        "off.condition",
        "off.delay",
    ]

    def __init__(self) -> None:
        super().__init__()
        self.definition = {}
        self.definition.update({
            "active": False,
            "waiting": False
        })

    def _checkRequiredFields(self, modeDict: dict = {}):
        for path in self._REQUIRED_FIELDS:
            keyPath = path.split('.')
            if misc.getFromDict(modeDict, keyPath) is None:
                raise AttributeError(f"Missing info : {path} from mode definition")
                return False
        return True

    def importModeDict(self, modeDict: dict = {}):
        if self._checkRequiredFields(modeDict):
            self.definition = modeDict

    def __str__(self) -> str:
        return self.definition

    def __repr__(self) -> str:
        return str(self.definition)


def loadModes():
    modesList = []
    modes = filesystem.readJson('V2/dev/modes.json')
    for modeDef in modes:
        _ = mode()
        _.importModeDict(modeDef)
        modesList.append(_)

    return modesList
